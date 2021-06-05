#include "Column.h"
#include <utils/Colors.h>

db::Column::Column(std::string name, COLUMN_T type, bool primaryKey, bool autoIncrement) : name(std::move(name)), type(type), primaryKey(primaryKey), autoIncrement(autoIncrement) {}

std::string db::Column::toString() const {
    std::string typeName;
    switch(type){
        case CT_STRING:
            typeName = std::string(BOLDBLUE) + "(string)";
            break;
        case CT_NUMBER:
            typeName = std::string(BOLDCYAN) + "(number)";
            break;
        default:
            typeName = std::string(BOLDRED) + "(unknown id=" + std::to_string(type) + ")";
    }

    typeName += RESET;

    return name + " " + typeName;
}
