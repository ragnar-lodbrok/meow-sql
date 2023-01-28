1. Add new language (if required) to meowsql.pro TRANSLATIONS
2. Gerenare ts files: lupdate meowsql.pro
3. Download Heidi's translates here: https://www.transifex.com/heidisql/heidisql/ui/
4. Convert to ts files (or use convert_all.sh): lconvert -locations relative for_use_heidisql_ui_fr.po -o heidisql_fr.ts
5. Copy translations from heidisql_*.ts to meowsql (copy_from_heidi)
6. lrelease meowsql.pro
