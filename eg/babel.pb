-- bvmexec.pb (rename to babel.pb)
--
-- You need to run:
--
--    perl babel eg/hello_world.pb
--
-- ... before this program will work

main:
    ( `"eg/hello_world.pb.bbl" slurp
    load
    babel
    `"Done\n" << )

