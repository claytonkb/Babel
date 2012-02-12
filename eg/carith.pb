-- carith.pb
-- This program demonstrates Babel's C-style arithmetic operators

ops: ((("cuadd" (u+)) ("cusub" (u-)) ("cumul" (cumul)) ("cudiv" (u/))
       ("ciadd" (+))  ("cisub" (-))  ("cimul" (cimul)) ("cidiv" (/))))

code:
    ---
         ( ((dup car inform !
            prompts !

            <-  <->
            ->  <->
            cdr car !

            ci2dec ("\n") . << ))

        ops ...)

inform: (((":\n") . << ))

prompts: (( prompt ! prompt ! ))

prompt:
   ((("Enter a number:\n> ") << 
   >> $d ))

