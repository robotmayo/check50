#include "ActivityReactor.h"

namespace Check50 {

  void ExecutionActivityReactor::on_status() {
    

    switch (_activity->status()) {
    case Activity::waiting:

      break;
    case Activity::ready:
      _manager->last_activity_is(_activity);
      break;
    default:
      break;
    }
  }

} /* end namespace Check50 */