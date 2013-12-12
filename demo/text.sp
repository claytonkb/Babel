-- text.sp
-- Strange Loop Conference 2013 ELC

((main (code 
    argv1 !
    >>> 
    str2ar ar2ls 
    newline_split !
    (code 
        dup
        ' ' ord char_match ! 
        split 
        (code 
            ls2lf ar2str nl <<)
        each) 
    each ))

(newline_split (code 
    dup
    0xa char_match ! 
    split ))

(char_match (code 
    match_list 2 newin 0 paste
    compare_char <-> set
    (code 
        (code compare_char =) 
            (code 
                match_list iter 
                1 take unshift zap) 
            (code fnord) 
        ifte)
    each
    match_list cdr))

(compare_char (val 0))

(match_list (ptr nil nil))

(tons (code take bons)))

