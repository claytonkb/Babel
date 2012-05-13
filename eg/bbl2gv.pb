-- bbl2gv.pb (need to rename this to dump.pb)
-- 
-- Ex:
-- perl babel eg/bbl2gv.pb > bbl2gv.dot
-- dot -Tsvg bbl2gv.dot > bbl2gv.svg
-- Open bbl2gv.svg in Chrome

main: ( `test dump << )

test: [{0xffffffff 0xbabeface} {0xdeadbeef 0xdecade} nil]

