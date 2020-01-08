#include "string_utils.h"

string_utils::string_utils()
{

}
bool string_utils::isBlank(QString s) {
    return s.trimmed() == "";
}
