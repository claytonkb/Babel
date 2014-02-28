 -- Babel opcode definitions...

(   (cushl          (oper 0x001))
    (cushr          (oper 0x003))
    (curol          (oper 0x005))
    (cashr          (oper 0x006))
    (curor          (oper 0x007))
    (cashr          (oper 0x00b))
    (cnot           (oper 0x00c))

    (cne            (oper 0x010))
    (~=             (oper 0x010))
    (ceq            (oper 0x011))
    (=              (oper 0x011))
    (cult           (oper 0x012))
    (u<             (oper 0x012))
    (cule           (oper 0x013))
    (u<=            (oper 0x013))
    (cugt           (oper 0x014))
    (u>             (oper 0x014))
    (cuge           (oper 0x015))
    (u>=            (oper 0x015))
    (cilt           (oper 0x01a))
    (<              (oper 0x01a))
    (cile           (oper 0x01b))
    (<=             (oper 0x01b))
    (cigt           (oper 0x01c))
    (>              (oper 0x01c))
    (cige           (oper 0x01d))
    (>=             (oper 0x01d))

    (logicF0        (oper 0x020))
    (logicF1        (oper 0x021))
    (cand           (oper 0x021))
    (&              (oper 0x021))
    (logicF2        (oper 0x022))
    (logicF3        (oper 0x023))
    (logicF4        (oper 0x024))
    (logicF5        (oper 0x025))
    (logicF6        (oper 0x026))
    (cxor           (oper 0x026))
    (^              (oper 0x026))
    (logicF7        (oper 0x027))
    (cor            (oper 0x027))
    (|              (oper 0x027))
    (logicF8        (oper 0x028))
    (cnor           (oper 0x028))
    (~|             (oper 0x028))
    (logicF9        (oper 0x029))
    (cxnor          (oper 0x029))
    (~^             (oper 0x029))
    (logicFa        (oper 0x02a))
    (logicFb        (oper 0x02b))
    (logicFc        (oper 0x02c))
    (logicFd        (oper 0x02d))
    (logicFe        (oper 0x02e))
    (cnand          (oper 0x02e))
    (~&             (oper 0x02e))
    (logicFf        (oper 0x02f))


    (cuadd          (oper 0x030))
    (u+             (oper 0x030))
    (cusub          (oper 0x031))
    (u-             (oper 0x031))
    (cumul          (oper 0x032))
    (u*             (oper 0x032))
    (cudiv          (oper 0x033))
    (u/             (oper 0x033))
    (curem          (oper 0x035))
    (u%             (oper 0x035))
    (ciadd          (oper 0x038))
    (+              (oper 0x038))
    (cisub          (oper 0x039))
    (-              (oper 0x039))
    (cimul          (oper 0x03a))
    (*              (oper 0x03a))
    (cidiv          (oper 0x03b))
    (/              (oper 0x03b))
    (ciabs          (oper 0x03c))
    (cirem          (oper 0x03d))
    (%              (oper 0x03d))

    (catoi          (oper 0x0a0))

    (cprintf        (oper 0x0c0))
    (cprints        (oper 0x0a8))

    (fnord          (oper 0x100))
    (babel          (oper 0x102))
    (!!             (oper 0x102))
    (msize          (oper 0x107))
    (arlen8         (oper 0x108))
    (#8             (oper 0x108))
    (free           (oper 0x109))
    (rmha           (oper 0x10a))
    (arlen          (oper 0x10b))
    (#              (oper 0x10b))
    (cat8           (oper 0x10c))
    (.              (oper 0x10c))
    (cat            (oper 0x10f))

    (mu             (oper 0x110))
    (nva            (oper 0x111))
    (npt            (oper 0x112))
    (nlf            (oper 0x113))
    (nin            (oper 0x114))
    (load           (oper 0x115))
    (unload         (oper 0x116))
    (bs2gv          (oper 0x117))
    (show           (oper 0x117))
    (slice8         (oper 0x118))
    (slice          (oper 0x11b))
    (arcmp          (oper 0x11f))
    (==             (oper 0x11f))

    (sleep          (oper 0x120))
    (die            (oper 0x121))
    (goto           (oper 0x122))
    (loop           (oper 0x125))
    (last           (oper 0x126))
    (next           (oper 0x127))
    (cut            (oper 0x12b))
    (trunc          (oper 0x12c))

    (eval           (oper 0x130))
    (!              (oper 0x130))
    (while          (oper 0x135))
    (times          (oper 0x136))
    (isinte         (oper 0x138))
    (newin          (oper 0x13a))
    (ar2ls          (oper 0x13b))
    (isleaf         (oper 0x13c))
    (del            (oper 0x13d))
    (newlf          (oper 0x13e))
    (sfield         (oper 0x13f))

    (hash8          (oper 0x140))
    (%%             (oper 0x140))
    (rand           (oper 0x141))
    (argv           (oper 0x143))
    (cp             (oper 0x148))
    (paste          (oper 0x149))

    (hash           (oper 0x150))
    (luha           (oper 0x151))
    (insha          (oper 0x152))
    (exha           (oper 0x154))
    (keysha         (oper 0x156))
    (entsha         (oper 0x156))

    (inskha         (oper 0x162))

    (ar2str         (oper 0x180))
    (str2ar         (oper 0x181))
    (cu2dec         (oper 0x184))
    (%u             (oper 0x184))
    (cu2hex         (oper 0x185))
    (%x             (oper 0x185))
    (ci2dec         (oper 0x186))
    (%d             (oper 0x186))
    (len            (oper 0x188))
    (##             (oper 0x188))

    (bbl2str        (oper 0x190))
    (hex2cu         (oper 0x195))
    ($x             (oper 0x195))
    (dec2ci         (oper 0x196))
    ($d             (oper 0x196))
    (uncons         (oper 0x198))
    (cdr            (oper 0x199))
    (pop            (oper 0x19a))
    (zap            (oper 0x19b))
    (swap           (oper 0x19e))
    (<->            (oper 0x19e))
    (sel            (oper 0x19f))

    (cons           (oper 0x1a8))
    (car            (oper 0x1a9))
    (push           (oper 0x1aa))
    (dup            (oper 0x1ab))
    (shift          (oper 0x1ae))
    (unshift        (oper 0x1af))

    (slurp          (oper 0x1b0))
    (spit           (oper 0x1b1))
    (ls2lf          (oper 0x1ba))
    (bons           (oper 0x1bb))
    (isnil          (oper 0x1bd))

    (slurp8         (oper 0x1c0))
    (>>>            (oper 0x1c0))
    (spit8          (oper 0x1c1))
    (<<<            (oper 0x1c1))
    (journal8       (oper 0x1c2))


    (stdout8        (oper 0x1e0))
    (<<             (oper 0x1e0))
    (stdinln        (oper 0x1e3))
    (>>             (oper 0x1e3))
    (nhref          (oper 0x1e5))
    (and            (oper 0x1e6))
    (&&             (oper 0x1e6))
    (or             (oper 0x1e7))
    (||             (oper 0x1e7))
    (not            (oper 0x1e8))
    (~~             (oper 0x1e8))
    (each           (oper 0x1ea))
    (...            (oper 0x1ea))
    (span           (oper 0x1ec))
    (eachar         (oper 0x1ed))
    (phash          (oper 0x1ee))
    (phash8         (oper 0x1ef))

    (down           (oper 0x1f0))
    (<-             (oper 0x1f0))
    (up             (oper 0x1f1))
    (->             (oper 0x1f1))
    (take           (oper 0x1f2))
    (depth          (oper 0x1f3))
    (give           (oper 0x1f4))
    (nest           (oper 0x1f5))
    (clear          (oper 0x1f6))
    (stdout         (oper 0x1f7))
    (trav           (oper 0x1f9))
    (@              (oper 0x1f9))
    (ith            (oper 0x1fa))
    (cr             (oper 0x1fb))
    (nl             (oper 0x1fb))
    (walk           (oper 0x1fe))
    (@@             (oper 0x1fe))
    (set            (oper 0x1ff))

    (perm           (oper 0x200))
    (add_assign     (oper 0x201))
    (bvmstack       (oper 0x202))
    (bvmcode        (oper 0x203))
    (ifte           (oper 0x204))
    (bvmustack      (oper 0x205))
    (rev            (oper 0x206))
    (boilerplate    (oper 0x207))
    (paste8         (oper 0x208))
    (iter           (oper 0x209))
    (conjure        (oper 0x20b))
    (self           (oper 0x20c))
    (sub_assign     (oper 0x20d))
    (endian         (oper 0x20f))

    (nhword         (oper 0x210))
    (deref          (oper 0x211))
    (if             (oper 0x212))
    (?              (oper 0x212))
    (ord            (oper 0x213))
    (let            (oper 0x214))
    (split          (oper 0x215))
    (lusym          (oper 0x216))
    (th             (oper 0x217))
    (move           (oper 0x218))
    (move8          (oper 0x219))
    (ins            (oper 0x21a))
    (exec           (oper 0x21b))
    (dev            (oper 0x21c))
    (cond           (oper 0x21d))
    (??             (oper 0x21d))
    (append         (oper 0x21e))
    (newha          (oper 0x21f))

    (bvmrstack      (oper 0x220))
    (bvmsym         (oper 0x221))
    (hibernate      (oper 0x222))
    (bvm_step       (oper 0x223))
    (rawmem         (oper 0x224))
    (set_ith        (oper 0x225))
    (set_car        (oper 0x226))
    (set_cdr        (oper 0x227))
    (detag          (oper 0x228))
    (flush_cache    (oper 0x229))
    (update_cache   (oper 0x22a))
    (copy_collect   (oper 0x22b))
    (bvm_root       (oper 0x22c))
    (dump_cache     (oper 0x22d))
    (sortlf         (oper 0x22e))
    (shuf           (oper 0x22f))

    (pass           (oper 0x230))
    (fail           (oper 0x231))
    (newtptr        (oper 0x232))
    (istptr         (oper 0x233))
    (get_tag        (oper 0x234)))

