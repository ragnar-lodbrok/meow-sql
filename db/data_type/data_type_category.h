#ifndef DB_DATA_TYPE_CATEGORY
#define DB_DATA_TYPE_CATEGORY


namespace meow {
namespace db {

enum class DataTypeCategoryIndex {
    None, // TODO: replace with Other?
    Integer,
    Float,// H: Real
    Text,
    Binary,
    Temporal,
    Spatial,
    Other
    // TODO: add Arrays
};

} // namespace db
} // namespace meow

#endif // DB_DATA_TYPE_CATEGORY

