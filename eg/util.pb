main:
    (   `"A random number: " <<
        `1 rand
        %d <<

        `"\nSleeping for 3 seconds... " <<
        `3 sleep
        `"Awake!\n" << 

        `"You passed " <<
        argv # `1 - %d <<
        `" arguments on the command-line\n" <<

        `"This version of Babel is " <<
        msize `8 cumul %d <<
        `" bits\n\n" << 

        boilerplate << )

