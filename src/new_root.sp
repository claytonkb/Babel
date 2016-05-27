-----------------------------------------------------------------------------
-- Help section
-----------------------------------------------------------------------------

{ help_msg
    ("\nTo launch the Babel quick-start guide enter:"
    ""
    "    quick_start !"
    ""
    "To launch the Babel advanced tutorial enter:"
    ""
    "    'tut.sp' include ! !"
    ""
    "Follow the on-screen directions. To load the manual for Babel operators, enter:"
    ""
    "    'man.sp' include ! !"
    ""
    "To see the man-page for 'cat', for example:"
    ""
    "    'cat' man !"
    ""
    "To see a list of all man-page topics, enter:"
    ""
    "    man_pages dirmap !\n")
    <

help { help_msg {<< "\n" <<} ... } <

cli_help_msg
    (""
    "To launch Babel in interactive mode:"
    "    babel.exe"
    ""
    "To launch Babel in interactive mode with arguments:"
    "    babel.exe -i <args>"
    ""
    "To execute a .sp file in non-interactive mode:"
    "    babel.exe <sp-filename>"
    ""
    "To execute a .bbl file in non-interactive mode:"
    "    babel.exe -b <bbl-filename>"
    "")
    <

cli_help { "cli_help_msg" lusym {<< "\n" <<} ... } <

courtesy_msg "For help, enter 'help !', without quotes; enter 'exit' to quit\n" <

std {'std.sp' >>> sexpr bpdli ! 'std.sp loaded /n' <<} <
quick_start {'quick.sp' include ! !} <
man_start   {'man.sp'   include ! !} <

--get_argv { root detag "/babel/sym/argv" hash luha 2 ith } <
get_argv { 2 dev detag "/babel/sym/argv" hash luha 2 ith } <

log_history {"\n\n" .+ "babel_history" journal} <

-- argv root detag "/babel/sym/argv" hash luha 2 ith pop zap < 

std !

-----------------------------------------------------------------------------
-- BVM construction begins
-----------------------------------------------------------------------------

    (("ustack_ptr"  (nil))
    ("soft_root"    [ptr nil nil])
    ("soft_root"    [tag "/babel/tag/trie" ptr () ()])
    ("rstack_ptr"   (nil))
    ("thread_id"    (1))
    ("advance_type" (0))
    ("bvm_initd"    (0))
    ("dstack_ptr"   (nil))
    ("local_root"   (nil))
    ("steps"        (-1)))

     <- newha ->
    {<- dup   ->
        dup  1 ith
        <-   0 ith ->
        inskha}
    each

-----------------------------------------------------------------------------
-- BVM construction finished
-----------------------------------------------------------------------------

    -- Check argv
    -- root detag "/babel/sym/argv" hash luha 2 ith dup
    get_argv ! dup

    is_nil

    { 0 ith
    '-i' arcmp

    ----- Interactive Mode with args -----

        {   argv
            get_argv ! pop zap
            <

            courtesy_msg <<

            zap
            { "{ "
            "babel> " <<
            stdinln8
            dup
            0 eq?
                { " }" .+ .+
                    sexpr bpdli
                    eval }
                { zap zap }
            selr eval }
        loop }

    ----- Run Mode -----
        { get_argv ! 0 ith '-h' arcmp not

            { get_argv ! 0 ith '-b' arcmp not

                { get_argv ! 0 ith >>> 
                sexpr bpdli
                mkbvm2 !

                dup detag "soft_root" hash luha 2 ith "argv" hash get_argv ! pop zap insha

                <- () -> babel zap }
       
                { get_argv ! 1 ith slurpm load <- () -> babel zap }
            selr eval }

            { cli_help ! }
        selr eval }

    selr eval } -- selects between Interactive Mode and Run Mode

    ----- Interactive Mode without args -----
    { courtesy_msg <<

    "\n----- Babel interactive session -----" log_history !

    zap zap
    { "{ "
        "babel> " <<
        stdinln8
        dup
        0 eq?
            { dup log_history ! -- "\n\n" .+ "babel_history" journal
                " }" .+ .+
                sexpr bpdli
                eval }
            { zap zap }
        selr eval }
    loop  }

    selr eval }

