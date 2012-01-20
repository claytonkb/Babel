code:
   ([(1 2 3)] car disp call
    [(1 2 3)] cdr disp call
    [(1)] [(2 3)] cons dup disp call
    uncons disp call disp call
    [(1 2 3)] uncons disp call disp call)

disp:
    [(bbl2str stdout ["\n"] stdout)]

