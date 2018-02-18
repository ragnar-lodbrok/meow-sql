#ifndef DB_DATA_TYPE_CATEGORY
#define DB_DATA_TYPE_CATEGORY


namespace meow {
namespace db {

enum class DataTypeCategoryIndex {
    None,
    Integer,
    Float,// H: Real
    Text,
    Binary,
    Temporal,
    Spatial,
    Other
};

} // namespace db
} // namespace meow

#endif // DB_DATA_TYPE_CATEGORY

