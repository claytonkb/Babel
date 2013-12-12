((main
   (code "car:\n" <<
   (list 1 2 3) car                     disp !

    "cdr:\n"       <<
    (list 1 2 3)            cdr         disp !

    "cons:\n"      <<
    1 (list 2 3)     cons dup    disp !

    "uncons:\n"    <<
                            uncons      disp !   
                                        disp !

    "uncons:\n"    <<
    (list 1 2 3)            uncons      disp ! 
                                        disp !

    "ar2ls:\n"     <<
    (ptr 1 2 3)             ar2ls dup   disp !

    "bons:\n"      <<
                            bons        disp !

    "push:\n"      <<
    (list 1 2)  (list 3)    push dup    disp !

    "pop:\n"       <<
                            pop         disp !
                                        disp !

    "unshift:\n"   <<
    (list 2 3)  (list 1)    unshift dup disp !

    "shift:\n"     <<
                            shift       disp !
                                        disp !))

(disp (code bbl2str << "\n" <<)))

