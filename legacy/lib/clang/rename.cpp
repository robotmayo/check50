//-------------------------------------------------------------------------
// rewrite.cpp: Source-to-source transformation with Clang that uses
// the code rewriting interface "Rewriter." Turns all references to the
// user-provided list of functions to _function, allowing us to throw in our
// own versions of those functions.
//
// NOTE: to compile in Clang 3.X, set the -DCLANG_3 flag.
//
// Author: Nate Hardison (nate@cs.harvard.edu)
// 
// Based largely off code written by Eli Bendersky (eliben@gmail.com) and
// posted at: http://eli.thegreenplace.net/2012/06/08/basic-source-to-source-transformation-with-clang/
//
// Also got significant help from the Clang tutorials at:
// https://github.com/loarabia/Clang-tutorial/wiki/TutorialOrig
//--------------------------------------------------------------------------
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <sstream>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Rewriter.h"
#include "clang/Rewrite/Rewriters.h"
#include "llvm/Support/Host.h"

using namespace clang;

class FunctionRefsRenamer : public RecursiveASTVisitor<FunctionRefsRenamer>
{
public:
    FunctionRefsRenamer(Rewriter &r,
                        std::set<std::string> &functions,
                        std::set<FileID> &files,
                        SourceManager &s) : rw(r), functions(functions), 
                                            files(files), sm(s) {}

    /**
     * Visits all function calls, rewriting if them if necessary.
     * We don't want to rewrite functions from system libraries (like printf).
     */
    bool
    VisitCallExpr(const CallExpr *ce)
    {
        // Grab the FuncDecl to make sure it's not from a system library and to
        // pull out the function name.
        const FunctionDecl *fd = ce->getDirectCallee();
        if (fd != NULL && !sm.isInSystemHeader(fd->getLocStart()))
        {
            // We get the name from the FuncDecl, but the location from the
            // CallExpr (we rename the declaration in VisitFunctionDecl).
            std::string name = fd->getNameInfo().getName().getAsString();
            SourceLocation location = ce->getLocStart();
            Rename(name, location);

        }
        return true;
    }

    /**
     * Visits all function declarations, renaming them if necessary.
     * Again, we avoid renaming functions from system libraries.
     */
    bool
    VisitFunctionDecl(const FunctionDecl *fd)
    {
        std::string name = fd->getNameInfo().getName().getAsString();
        SourceLocation location = fd->getLocStart();
        if (!sm.isInSystemHeader(location))
            Rename(name, location);
        return true;
    }

private:
    Rewriter &rw;
    std::set<std::string> &functions;
    std::set<FileID> &files;
    SourceManager &sm;

    /**
     * We prepend a '_' to each function reference to make sure that there
     * aren't any name conflicts with our own functions. Rename also 
     */
    void
    Rename(std::string name, SourceLocation location)
    {
        if (functions.empty() || functions.find(name) != functions.end())
        {
            rw.InsertTextBefore(location, "_");
            files.insert(sm.getFileID(location));
        } 
    }
};

class FunctionRefsConsumer : public ASTConsumer
{
public:
    FunctionRefsConsumer(Rewriter &rw,
                         std::set<std::string> &functions,
                         std::set<FileID> &files,
                         SourceManager &sm) : renamer(rw, functions, files, sm) {}

// hack hack hack
#ifdef CLANG_2
    virtual void
#else
    virtual bool
#endif
    HandleTopLevelDecl(DeclGroupRef dr)
    {
        for (DeclGroupRef::iterator b = dr.begin(), e = dr.end(); b != e; ++b)
            renamer.TraverseDecl(*b);
#ifndef CLANG_2
        return true;
#endif
    }

private:
    FunctionRefsRenamer renamer;
};

int
main(int argc, char *argv[])
{
    if (argc < 2)
    {
        llvm::errs() << "Usage: " << argv[0];
        llvm::errs() << " <filename> [function [function ...]]\n";
        return 1;
    }

    // make sure we got a legit file arg
    struct stat sb;
    if (stat(argv[1], &sb) == -1)
    {
      perror(argv[1]);
      exit(EXIT_FAILURE);
    }

    // TODO: this should really be a set of regexes since we want to make sure
    // that renaming main also renames _main if it exists (and renaming _main
    // renames __main if it exists, ad infinitum).
    std::set<std::string> functions;
    for (int i = 2; i < argc; i++)
        functions.insert(argv[i]);

    // This stores the FileIDs of all files that get rewritten somehow.
    // We need FileIDs to get the RewriteBuffers with the new contents later.
    std::set<FileID> files;

    // --------------------------------------- //
    // !!!!!!!!!! CRAZY CLANG STUFF !!!!!!!!!! //
    // --------------------------------------- //

    // CompilerInstance will hold the instance of the Clang compiler for us,
    // managing the various objects needed to run the compiler.
    CompilerInstance ci;
    ci.createDiagnostics(0, 0);

    // Initialize target info with the default triple for our platform.
    TargetOptions to;
#ifdef CLANG_2
    to.Triple = llvm::sys::getHostTriple();
#else
    to.Triple = llvm::sys::getDefaultTargetTriple();
#endif

    TargetInfo *ti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(), to);
    ci.setTarget(ti);

    ci.createFileManager();
    FileManager &fm = ci.getFileManager();
    ci.createSourceManager(fm);
    SourceManager &sm = ci.getSourceManager();
    ci.createPreprocessor();
    ci.createASTContext();

    // A Rewriter helps us manage the code rewriting task.
    Rewriter rw;
    rw.setSourceMgr(sm, ci.getLangOpts());

    // Set the main file handled by the source manager to the input file.
    const FileEntry *fileIn = fm.getFile(argv[1]);
    sm.createMainFileID(fileIn);
    ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                             &ci.getPreprocessor());

    // Create an AST consumer instance which is going to get called by
    // ParseAST.
    FunctionRefsConsumer mrc(rw, functions, files, sm);

    // Parse the file to AST, registering our consumer as the AST consumer.
    ParseAST(ci.getPreprocessor(), &mrc, ci.getASTContext());

    ci.getDiagnosticClient().EndSourceFile();

    // --------------------------------------- //
    // !!!!!!!! END CRAZY CLANG STUFF !!!!!!!! //
    // --------------------------------------- //

    // Write the rewritten results out to new files
    std::set<FileID>::iterator b = files.begin(), e = files.end();
    for ( ; b != e; b++)
    {
        // Get the appropriate RewriteBuffer using the FileID
        const RewriteBuffer *rb = rw.getRewriteBufferFor(*b);

        // I can't believe it's this absurdly hard to get a filename from
        // a FileID...
        PresumedLoc ploc = sm.getPresumedLoc(sm.getLocForStartOfFile(*b));
        std::string filename = ploc.getFilename();

        std::ofstream file;
        file.open((filename + ".renamed").c_str());
        file << std::string(rb->begin(), rb->end()) << "\n";
        file.close();
    }

    return 0;
}