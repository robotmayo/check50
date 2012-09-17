//-------------------------------------------------------------------------
// remove-main.cpp: Rips out the definition of the main function in the
// user-provided .c or .cpp file.
//
// NOTE: in Clang < 3.0, we can't get the location of the return type of the
// main function, so unfortunately the "int" in "int main(...)" won't be
// removed.
//
// NOTE: to compile in Clang 2.X, set the -DCLANG_2 flag.
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

#include <iostream>
#include <string>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Parse/Parser.h"
#include "clang/Rewrite/Rewriter.h"

#include "llvm/Support/Host.h"

using namespace clang;

class RemoveMainConsumer : public ASTConsumer
{
public:
    RemoveMainConsumer(Rewriter &rw, SourceManager &sm) : rw(rw), sm(sm) {}

// hack hack hack hack hack
#ifdef CLANG_2
    virtual void
#else
    virtual bool
#endif
    HandleTopLevelDecl(DeclGroupRef dgr)
    {
        for (DeclGroupRef::iterator it = dgr.begin(); it != dgr.end(); it++)
        {
            if (const FunctionDecl *fd = llvm::dyn_cast<FunctionDecl>(*it))
            {
                // use hasBody(fd) to reset fd to point to actual declaration
                // of main in case it's been prototyped separately
                if (fd->isMain() && fd->hasBody(fd))
                {
                    // why, oh why can't you just subtract SourceLocations?
                    unsigned begin = sm.getFileOffset(fd->getLocStart());
                    unsigned end = sm.getFileOffset(fd->getLocEnd());
                    unsigned length = end - begin + 1;
        
                    // need a SourceLocation for first arg, not a file offset
                    rw.RemoveText(fd->getLocStart(), length);
                }
            }
        }
// me loves the preprocessor
#ifndef CLANG_2
        return true;
#endif
    }

private:
    Rewriter &rw;
    SourceManager &sm;
};

int
main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        llvm::errs() << "Usage: " << argv[0] << " <filename>\n";
        exit(1);
    }

    // make sure we got a legit file arg
    struct stat sb;
    if (stat(argv[1], &sb) == -1)
    {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    // --------------------------------------- //
    // !!!!!!!!!! CRAZY CLANG STUFF !!!!!!!!!! //
    // --------------------------------------- //

    CompilerInstance ci;
    ci.createDiagnostics(0,NULL);

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
    ci.getPreprocessorOpts().UsePredefines = false;

    // Set up the rewriter
    Rewriter rw;
    rw.setSourceMgr(sm, ci.getLangOpts());

    RemoveMainConsumer *consumer = new RemoveMainConsumer(rw, sm);
    ci.setASTConsumer(consumer);

    ci.createASTContext();

    const FileEntry *file = fm.getFile(argv[1]);
    sm.createMainFileID(file);
    ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                             &ci.getPreprocessor());
    ParseAST(ci.getPreprocessor(), consumer, ci.getASTContext());
    ci.getDiagnosticClient().EndSourceFile();

    // --------------------------------------- //
    // !!!!!!!! END CRAZY CLANG STUFF !!!!!!!! //
    // --------------------------------------- //

    // Print the contents of the RewriteBuffer to stdout
    const RewriteBuffer *rb = rw.getRewriteBufferFor(sm.getMainFileID());
    if (rb != NULL)
        llvm::outs() << std::string(rb->begin(), rb->end()) << "\n";

    return 0;
}
