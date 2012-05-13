main: ( `"give and take:\n" <<
        `(1 2 3) give 
        stack_disp !

        `"swap:\n" <<
        `4 `5 <-> 
        stack_disp !

        `"up and down:\n" <<
        `6 <- `7 -> 
        stack_disp !

        `"up/down/swap:\n" <<
        `1 `2 `3 <- <-> -> 
        stack_disp !

        `"zap:\n" <<
        `4 `5 zap
        stack_disp !

        `"up/down/zap:\n" <<
        `6 `7 <- zap ->
        stack_disp !

        `"dup:\n" <<
        `8 dup
        stack_disp !

        `"sel (false):\n" <<
        `0 `8 `9 sel
        stack_disp !

        `"sel (true):\n" <<
        `1 `8 `9 sel
        stack_disp !

        `"depth:\n" <<
        `1 `1 `1 `1 `1 depth
        stack_disp !    

        `"multiple up/down:\n" <<
        `1 `1 `1 `1 `1 `1 `1 <- <- <- <- `2 -> `3 `4 -> `5 -> `6 -> `7 `8
        stack_disp ! )

stack_disp: (( `-1 take reverse <- `( disp ! ) -> ... ))

disp: (( $ cr << ))


