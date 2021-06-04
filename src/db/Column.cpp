#include "Column.h"

db::Column::Column(std::string name, COLUMN_T type, bool primaryKey, bool autoIncrement) : name(std::move(name)), type(type), primaryKey(primaryKey), autoIncrement(autoIncrement) {}

std::string db::Column::toString() const {
    std::string typeName;
    switch(type){
        case CT_STRING:
            typeName = "(string)";
            break;
        case CT_NUMBER:
            typeName = "(number)";
            break;
        default:
            typeName = "(unknown id=" + std::to_string(type) + ")";
    }

    return name + " " + typeName;
}
