package grgr.adventofcode2017;

import java.io.BufferedReader;
import java.io.StringReader;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertTrue;

public class Day07Test {

    String input1 = "pbga (66)\n" +
            "xhth (57)\n" +
            "ebii (61)\n" +
            "havc (66)\n" +
            "ktlj (57)\n" +
            "fwft (72) -> ktlj, cntj, xhth\n" +
            "qoyq (66)\n" +
            "padx (45) -> pbga, havc, qoyq\n" +
            "tknk (41) -> ugml, padx, fwft\n" +
            "jptl (61)\n" +
            "ugml (68) -> gyxo, ebii, jptl\n" +
            "gyxo (61)\n" +
            "cntj (57)\n";
    String input = "cfkcj (74)\n" +
            "kmwhbm (32)\n" +
            "uuvdgc (58)\n" +
            "siyms (98)\n" +
            "jgtvhkv (1885) -> ykqcpiv, gvupyd, vuyxvq\n" +
            "ppdypq (86)\n" +
            "lovxjut (90) -> fmvna, ddneaes, sakwdmk, lqmoz\n" +
            "ijnvdm (98)\n" +
            "ibzlhq (86)\n" +
            "pabuo (81)\n" +
            "brexb (75) -> tbmiv, iwrph\n" +
            "uqiwwa (705) -> inriw, rnelci\n" +
            "psyroai (498) -> porhb, qinqu, esxiq\n" +
            "ldkxt (69)\n" +
            "mzfbeo (68)\n" +
            "spjtt (299) -> xfcxpkh, sizgye\n" +
            "dnjao (243)\n" +
            "rqwnb (9)\n" +
            "jfnfq (94)\n" +
            "giuonbb (68)\n" +
            "gbywwf (103) -> ugskjw, gelpv\n" +
            "fldjrj (52)\n" +
            "vuxzudl (31)\n" +
            "ewnboyh (38)\n" +
            "bolsi (11)\n" +
            "gbziz (33)\n" +
            "ytkppl (95)\n" +
            "rahuwnt (86)\n" +
            "toprb (282) -> dzyvcxt, xlyngh, tkhbr, avufn, uhhiz, tmtqgn\n" +
            "jadnvpr (13)\n" +
            "rnsotgi (47)\n" +
            "ovcgnvp (309) -> zlffm, iqwjlgh\n" +
            "xpbro (69)\n" +
            "tjulok (12)\n" +
            "pvylfqc (199) -> pzybx, nbdewmj\n" +
            "smwdyt (22)\n" +
            "oozzs (55) -> vrqgow, iynmj\n" +
            "qmzcp (89)\n" +
            "ssneqxd (22)\n" +
            "fqhdfwi (1041) -> ytjhh, rqpbxi, uvprg\n" +
            "bgsbjcq (81)\n" +
            "ilmvjcn (7)\n" +
            "adnrrap (69)\n" +
            "tspxgun (48)\n" +
            "tbmiv (77)\n" +
            "tceqj (58)\n" +
            "nlptf (157) -> vpvgnt, memsb, mcsoi, ctnckd\n" +
            "yzaveth (320) -> frfkpn, viockjp, cqzvhh\n" +
            "dgwely (74)\n" +
            "dpjpifo (47)\n" +
            "lncbnc (238) -> bmiqc, uvnmx\n" +
            "ppgvqkj (48)\n" +
            "fdbsgs (31)\n" +
            "usjpp (55) -> cxhms, niixavl, bspsg, htwyil, pyavtyz\n" +
            "ovhxqx (53)\n" +
            "vzyut (17)\n" +
            "pvxvvv (52)\n" +
            "gkouyvq (73)\n" +
            "ugjmo (86)\n" +
            "mblsyx (51158) -> vqpzey, bavhyuz, fqwyfso\n" +
            "vyrwv (99)\n" +
            "gdjawl (10)\n" +
            "vxcjbsi (56)\n" +
            "tiedml (44)\n" +
            "ihxixk (61) -> pduyk, betaiu\n" +
            "pvthg (41)\n" +
            "gehqj (77)\n" +
            "luxlkxu (1170) -> bzlttq, rowhly, nlptf\n" +
            "irckle (65) -> kuvzyp, nkotu, uqfiwz\n" +
            "fjwfhq (73)\n" +
            "drzukuh (55)\n" +
            "pzyxbdy (81)\n" +
            "brxrm (51)\n" +
            "dirdh (56)\n" +
            "ovujcr (13)\n" +
            "usfiw (7750) -> dtvjn, jektxhw, sfmvh\n" +
            "ytjhh (219) -> jcooqy, gvuvmfz\n" +
            "umnhed (1311) -> lefse, brexb, bexmjvo\n" +
            "vzvwks (52)\n" +
            "ydzdome (10)\n" +
            "iekhet (68)\n" +
            "jcooqy (5)\n" +
            "pcvok (38)\n" +
            "gqmqt (675) -> udebyn, agifkpl, gogsic\n" +
            "gkyvjtj (69)\n" +
            "uhhiz (88) -> bojcjr, krhtjha\n" +
            "jenzlcn (96)\n" +
            "mjntxj (48) -> mdhduoi, aztaw, tfkse\n" +
            "xgelkov (12)\n" +
            "rpadzt (24)\n" +
            "uoewbgw (54)\n" +
            "hmroizo (1564) -> hyjba, qetylot\n" +
            "qinqu (78) -> pabuo, bgsbjcq\n" +
            "cuikvge (150) -> vuxzudl, fdbsgs\n" +
            "jqjyspz (91)\n" +
            "dcaot (27) -> dnjao, bwbufij, mlzxc, auawgd, laxqy, awzuv, pvylfqc\n" +
            "yheomy (69)\n" +
            "kvspfv (81)\n" +
            "uoxqwap (161)\n" +
            "nfefd (82)\n" +
            "wfgammo (97)\n" +
            "mifrxb (158)\n" +
            "ihvae (5)\n" +
            "xfcxpkh (12)\n" +
            "pjegzrx (63) -> pgigk, zijedtv, ghdgob\n" +
            "xrxabjj (45)\n" +
            "fqmjnw (58)\n" +
            "pjgwz (99)\n" +
            "uxzazjg (64)\n" +
            "seeqovh (25)\n" +
            "eyxbeon (35)\n" +
            "umdhwh (27)\n" +
            "sesui (38)\n" +
            "nhnwqgh (59)\n" +
            "bnnfxqe (12)\n" +
            "blivyc (55)\n" +
            "ilrkhhd (84)\n" +
            "kzdxtck (37)\n" +
            "gfsmrr (172) -> xkuubv, eyxbeon\n" +
            "atwwiy (67)\n" +
            "rnelci (99)\n" +
            "hxlve (68)\n" +
            "pkwfv (32)\n" +
            "jnack (61)\n" +
            "pqqxhy (235) -> xsiklw, hmpcc\n" +
            "wcehmi (167) -> ygukv, upllk, kkdvdz, dqaum, synets\n" +
            "onwpfb (77)\n" +
            "zledwkk (7)\n" +
            "mjaqsj (78)\n" +
            "qsoybe (67)\n" +
            "eidhz (91)\n" +
            "blnsa (68) -> hftkbw, snllz, ddfnw, gefhbss\n" +
            "cswrj (176) -> laydujd, hpdegx\n" +
            "jtwckf (71)\n" +
            "lleyk (243) -> nyfnss, veujpah\n" +
            "synets (89) -> uuvdgc, vxqvj\n" +
            "swqrnfi (500) -> ivohs, zzqwyx, sxuafke\n" +
            "oyxlg (11)\n" +
            "zuruj (5)\n" +
            "loaex (63) -> jtwckf, enxsaag\n" +
            "mviybt (27)\n" +
            "qrxvlk (369) -> bpbfd, gdbapcx\n" +
            "yagjnwy (78)\n" +
            "ftlrjo (91)\n" +
            "zklwp (56555) -> kaursj, lahahn, ciwjgw\n" +
            "dehbyec (51)\n" +
            "udoabw (123) -> ewgjf, icdyrl\n" +
            "bojcjr (34)\n" +
            "dwmkyes (119) -> schbl, finmnfn, iiropvx\n" +
            "plhno (14)\n" +
            "bsfpjtc (42) -> zklwp, zoibnsq, mblsyx, vkkwtq, cwluofv\n" +
            "thfdorc (48) -> ieuus, resill, agiybx, lleyk, mqruxqs\n" +
            "hupbun (52)\n" +
            "nyfnss (14)\n" +
            "nkhiz (51)\n" +
            "gicaag (50)\n" +
            "kgsyd (92) -> pzyxbdy, uozna, bcxexao, kvspfv\n" +
            "fuopub (65) -> vqijdaj, mzfbeo\n" +
            "wekwg (51)\n" +
            "vuyxvq (55) -> rsaruxl, blukof\n" +
            "awzuv (133) -> kxbjrca, drzukuh\n" +
            "onrbqjg (314) -> qulqh, wekwg\n" +
            "gefhbss (171) -> rhfpsr, oziwk\n" +
            "gkcpihl (88)\n" +
            "nwbomv (87)\n" +
            "betaiu (82)\n" +
            "wuhui (66)\n" +
            "ocopf (53)\n" +
            "jjceeu (65)\n" +
            "gurmk (64)\n" +
            "ymrkr (77)\n" +
            "grupgc (24)\n" +
            "ucqgh (75)\n" +
            "ujdishy (60)\n" +
            "mrwgq (85)\n" +
            "buvuq (3294) -> usjpp, swqrnfi, oqdntf\n" +
            "cqzvhh (9)\n" +
            "ikhbgx (54)\n" +
            "ddfnw (77) -> blhurw, qdeybxh, dehbyec, hkhvjq\n" +
            "xnfjw (17)\n" +
            "nfrbj (25)\n" +
            "zdrbia (250) -> dpjpifo, rnsotgi\n" +
            "wkennss (43) -> xcobqnc, lpogx, jjceeu\n" +
            "njlyt (65)\n" +
            "thnxnda (66)\n" +
            "xkdua (12)\n" +
            "frfkpn (9)\n" +
            "uwlpeon (30)\n" +
            "gwuxmq (89) -> rladyxh, mjaqsj, ozkrca\n" +
            "lnxtx (91)\n" +
            "djpkm (75)\n" +
            "gbnmi (21) -> mviybt, umdhwh, nswhn\n" +
            "nswhn (27)\n" +
            "kkktvk (32)\n" +
            "rgzkfeq (275)\n" +
            "hcywt (26)\n" +
            "hbjiz (129) -> ldkxt, tvshylj\n" +
            "fkzwy (16)\n" +
            "aztaw (10) -> yfrcezm, kqglah, ytkppl, puxzdr\n" +
            "ocztn (95)\n" +
            "jbsdgwa (34)\n" +
            "fqwyfso (34) -> vuqlmx, hmroizo, fqhdfwi, ksvwst, hihvghh, dcaot\n" +
            "urnycch (8)\n" +
            "uyhqy (23)\n" +
            "resill (127) -> uvptuv, luzuu\n" +
            "udebyn (181)\n" +
            "qetylot (82)\n" +
            "hdjgig (51)\n" +
            "jntmriy (90)\n" +
            "rfvtvw (46)\n" +
            "lsfjnsb (186) -> azmknz, qrriz\n" +
            "vnantj (71) -> avxjqab, oxjor, dattu, nwbomv\n" +
            "epxtul (12) -> aiyvkt, yhmavn\n" +
            "ysqwf (161) -> mxumuj, zbjhd\n" +
            "jieqymv (77)\n" +
            "rqqirx (91)\n" +
            "xoqqhw (346) -> uigna, qfequa\n" +
            "qdrbdcf (59)\n" +
            "uufted (217) -> megix, vewew, ovujcr\n" +
            "frkzri (15)\n" +
            "ddjyphd (66)\n" +
            "modlepu (59)\n" +
            "ynbhq (17)\n" +
            "jsdqcw (7)\n" +
            "dpevfm (7)\n" +
            "tqmkur (63)\n" +
            "bzhnu (150) -> qcpdn, ymjhqjw\n" +
            "sbaeoqo (58)\n" +
            "vtprv (39)\n" +
            "cshts (7)\n" +
            "tlitf (70)\n" +
            "ynodbxy (58)\n" +
            "arfft (259) -> eebqhug, jggld\n" +
            "rxrayit (22)\n" +
            "vdymnhw (37)\n" +
            "uvptuv (72)\n" +
            "fjueeps (17)\n" +
            "tollmm (100) -> qjipfjv, glbhpwz, pkfyhk\n" +
            "emwqaz (5)\n" +
            "unehvbu (147) -> tjulok, fwapzg\n" +
            "wlqagku (34)\n" +
            "unikoz (47)\n" +
            "vizehnk (97)\n" +
            "lnotuxj (198) -> feyiw, vrcfi\n" +
            "tkneufh (51)\n" +
            "usfag (80)\n" +
            "lgwut (308) -> ohdwce, uoewbgw\n" +
            "hfpcxco (85)\n" +
            "qddkwld (233) -> fnzboe, lbrknfc, plhno\n" +
            "ksvwst (1053) -> gzwkbw, ihxixk, vxzowjy\n" +
            "camosg (85)\n" +
            "onbizml (64)\n" +
            "rrlifdx (21)\n" +
            "pzybx (22)\n" +
            "mhbhg (50)\n" +
            "upksvnf (8)\n" +
            "utxkvw (52)\n" +
            "mmhjhh (20)\n" +
            "gjqcj (64)\n" +
            "xljdooj (37)\n" +
            "aejrdpp (201) -> zopkd, beoww\n" +
            "fiutgke (17)\n" +
            "qnmuc (202) -> mmhjhh, sfyps\n" +
            "aqjrxbd (40) -> mrwgq, hfpcxco, kozlucm\n" +
            "imtxkez (94) -> cwthmnc, mzvyddo, rmvgczw, jgtvhkv, hejlvq, ihxivjd\n" +
            "daaexz (199)\n" +
            "ghdgob (86)\n" +
            "owqupte (43) -> vkuvc, ucwwc\n" +
            "eztix (16)\n" +
            "pudsuxc (20)\n" +
            "vlwnw (90)\n" +
            "nlotooe (89) -> azzfba, ghbfbn, ppdypq\n" +
            "xqdpgzt (66)\n" +
            "hzain (46)\n" +
            "tfjzhz (88) -> mhpsnlv, kpjzc\n" +
            "gihhi (20)\n" +
            "gvuvmfz (5)\n" +
            "hmpcc (38)\n" +
            "lotngsa (12)\n" +
            "rupou (1090) -> tcgqf, onbizml\n" +
            "xxjcew (40)\n" +
            "yxfyvw (26) -> nlotooe, kqcmyr, yzaveth, fsqkr, kryid\n" +
            "xlyngh (132) -> qznfyv, lotngsa\n" +
            "rjbaqwb (25)\n" +
            "ibhzmtf (10)\n" +
            "kqglah (95)\n" +
            "hwmegb (845) -> xmlzuy, nehvflm, rnpwyut\n" +
            "sfhrl (240)\n" +
            "fbxcht (284)\n" +
            "xmlzuy (90) -> tspxgun, ppgvqkj\n" +
            "npsgry (85) -> pdttnl, aqzkz, negvvn, blivyc\n" +
            "kxbjrca (55)\n" +
            "jrtqzng (12)\n" +
            "tcgqf (64)\n" +
            "rcmogxy (169) -> xbxnqfz, lkipow\n" +
            "gvfgvtd (124) -> vdymnhw, twyuae, fjhgrk\n" +
            "brzgxi (588) -> untbk, nawhukx, pelersw\n" +
            "cxfgjp (57)\n" +
            "xxqaj (250) -> irsxut, hmdmy\n" +
            "tldocc (42)\n" +
            "icdcn (73)\n" +
            "jtmhpm (44) -> cynginx, pjgwz\n" +
            "tdtiqm (135) -> hymwone, oejnh\n" +
            "qybpba (125) -> elknfln, ctmtk\n" +
            "sozke (24)\n" +
            "uvnmx (9)\n" +
            "hcovonm (56)\n" +
            "yzpxkx (62)\n" +
            "jyfkl (89)\n" +
            "kdhmnh (27) -> uljmqrb, ynxvst\n" +
            "dajvk (245) -> abjhq, qkdikgs\n" +
            "rwvvlbx (69) -> zligfrr, ttmotv\n" +
            "skrlqr (217) -> ibhzmtf, xodpco\n" +
            "ugpaod (27)\n" +
            "gxtabbr (85)\n" +
            "ipqome (26)\n" +
            "dattu (87)\n" +
            "zxozizh (96)\n" +
            "hgtwnxu (12) -> mtkquru, qdzefux, wccvu, qrnztia, etblz\n" +
            "zlkaugz (7) -> uyxisy, faifx, njiof, prkspdn, lovxjut, fatahc, kxxqxm\n" +
            "auawgd (153) -> vnuwn, hugzzwo\n" +
            "ehynzhf (99)\n" +
            "kvrbtlv (1024) -> wkennss, lnotuxj, wbzccs, hlijr, gkvdu\n" +
            "xyvst (196) -> upksvnf, rvdntii\n" +
            "okzaj (208) -> rgyofuq, bdzqyu\n" +
            "rsvtgxk (32)\n" +
            "thzgmn (33)\n" +
            "fxjooi (12)\n" +
            "extfuy (291) -> tnvblm, ljxeb\n" +
            "hazdl (58)\n" +
            "zisnz (66)\n" +
            "ukxuw (66)\n" +
            "mllmqhl (90)\n" +
            "hihvghh (48) -> kbyhru, xkyjdv, okzaj, dsqvto, sfhrl, trjerm, ocatfcc\n" +
            "xodpco (10)\n" +
            "fkqlr (94)\n" +
            "anhrt (14) -> jntmriy, mllmqhl, rmdui, tuorl\n" +
            "mytpi (432) -> inthrl, ffcivin, auijtpl\n" +
            "zvctpo (45)\n" +
            "ttmotv (84)\n" +
            "hftkbw (281)\n" +
            "zijedtv (86)\n" +
            "sksjqz (277) -> zghku, skykdl\n" +
            "vpvgnt (10)\n" +
            "odqxcfl (93)\n" +
            "qfequa (14)\n" +
            "tnvblm (7)\n" +
            "gkfxnzo (17)\n" +
            "pkfyhk (58)\n" +
            "kuypvuy (67)\n" +
            "uhavg (1034) -> aqjrxbd, aafpdpe, eoluf, opbjmvf\n" +
            "acxrgal (287) -> jholt, rqwnb\n" +
            "scengkj (97)\n" +
            "taausp (47)\n" +
            "rxfnzf (18)\n" +
            "uljmqrb (67)\n" +
            "zbvvekj (67)\n" +
            "zwzhv (270) -> yxpzp, pzwxz\n" +
            "gelpv (17)\n" +
            "kunxx (67)\n" +
            "etblz (273) -> dpevfm, kojrycd, lhbuca, jsdqcw\n" +
            "xqfwvbf (20)\n" +
            "ghiqkm (192) -> onwpfb, amqvafo\n" +
            "mfdhdz (99)\n" +
            "fdztdbr (1063) -> sqzdg, hlcre, uyiaqu, usttmag\n" +
            "opnqtf (10)\n" +
            "fsqkr (165) -> jggsgh, xwtls\n" +
            "qrriz (35)\n" +
            "dtvjn (1287) -> gvfgvtd, khajtz, hkyiv\n" +
            "xsyepmg (35) -> camosg, gxtabbr\n" +
            "khykj (37) -> jfnfq, fkqlr\n" +
            "pduyk (82)\n" +
            "aqzkz (55)\n" +
            "merswry (32)\n" +
            "wevnd (64)\n" +
            "blukof (37)\n" +
            "fzhncm (327) -> qybpba, pwtlh, gbywwf, ymbtxzb, dwmkyes\n" +
            "nbdewmj (22)\n" +
            "hmdmy (13)\n" +
            "slnbq (78)\n" +
            "ozkrca (78)\n" +
            "vnoztu (96)\n" +
            "eksshr (914) -> chcsjda, udoabw, fuopub\n" +
            "fnzboe (14)\n" +
            "irsxut (13)\n" +
            "qvyqnuk (132) -> wfmql, rxrayit\n" +
            "ewgjf (39)\n" +
            "kdcgzbw (75)\n" +
            "bbytzn (1597) -> lqwwuqk, shypdye, javnv\n" +
            "ljxeb (7)\n" +
            "vkuvc (78)\n" +
            "kbyhru (58) -> eidhz, lnxtx\n" +
            "cigbnvp (37)\n" +
            "nubyqxj (56)\n" +
            "mtlapd (73) -> veeinqe, nlbnj, jadnvpr\n" +
            "feiowiw (1956) -> zcibgd, chpsgen, slimx\n" +
            "pdfjuw (71) -> fllyd, ghdqy, vzcvxt, keesuec\n" +
            "vrcfi (20)\n" +
            "jggld (83)\n" +
            "wrsdsa (7)\n" +
            "kmzeahn (178) -> gkfxnzo, fiutgke\n" +
            "elknfln (6)\n" +
            "kxbclr (30)\n" +
            "wrrwam (180) -> scengkj, vizehnk\n" +
            "hlfgoa (111) -> wfgammo, rolegtf\n" +
            "uyxisy (566) -> vgicm, mnpow, urnnskw, rcmogxy\n" +
            "brtvs (188) -> ftumhcp, xkdua\n" +
            "jvyscs (126) -> smwdyt, ttegw\n" +
            "jwenagh (25)\n" +
            "hlijr (238)\n" +
            "zghku (22)\n" +
            "ykqcpiv (129)\n" +
            "dpmsnm (971) -> mifrxb, fdpsrru, mqvstxt, rxarqvt, vbypq\n" +
            "vgicm (113) -> rvwtrl, mhbhg\n" +
            "izjxyb (37)\n" +
            "bfanbgh (128) -> arfft, xfvfo, nitrl\n" +
            "hkykap (26)\n" +
            "cpbxfaf (11)\n" +
            "wrbqnx (39)\n" +
            "pwtlh (63) -> izjxyb, kzdxtck\n" +
            "tfpqwb (48)\n" +
            "htkhsaa (74)\n" +
            "fvdsi (40)\n" +
            "iwexk (50)\n" +
            "mtkquru (70) -> mdcnazp, ymrkr, gehqj\n" +
            "pxbjhvu (96) -> nubyqxj, bvxxoux, hcovonm, wxjuvo\n" +
            "jeopi (71)\n" +
            "oqdntf (335) -> ocwyg, afdox, axcgjhj\n" +
            "bveca (138)\n" +
            "bexmjvo (99) -> njlyt, uxupys\n" +
            "xteuk (74)\n" +
            "upllk (131) -> cigbnvp, pewlflc\n" +
            "lahahn (2750) -> bbytzn, fzvctf, utnrb\n" +
            "ycpff (564) -> qgyngw, hwhleil, obumrhe, rgzkfeq, qddkwld, nykzm\n" +
            "usttmag (266)\n" +
            "axcgjhj (291) -> ddroz, ilmvjcn\n" +
            "tdmpe (77) -> buxlsok, ufense, klkyckz\n" +
            "kchcxe (96)\n" +
            "fkksmck (11)\n" +
            "fzvctf (85) -> xgydg, hgyhzc, pqqxhy, qadkf, nsxfciq, gbuhi\n" +
            "mqwdr (2656) -> kvrbtlv, uhavg, tsndvs, ycpff, yadfos\n" +
            "pulsq (84)\n" +
            "giugg (141) -> fieeic, uocco, msjyqjc, xrvdagj\n" +
            "dgdeleb (35)\n" +
            "qyixlaf (25)\n" +
            "wrbrkm (193) -> hdjgig, brxrm, nkhiz\n" +
            "ddpua (95) -> fspnagx, tqmkur, xwfnl\n" +
            "eoluf (39) -> gjqcj, uxzazjg, wevnd, laepzgh\n" +
            "kgfeyyg (45)\n" +
            "ovqpta (602) -> hbjiz, wbrop, ihyawsa\n" +
            "kqkyaey (25)\n" +
            "uigna (14)\n" +
            "qulqh (51)\n" +
            "kdwsl (96)\n" +
            "fkurfvl (15)\n" +
            "auijtpl (44) -> kdtiuq, ijnvdm\n" +
            "wzdgidb (10270) -> xqqgyu, osydb, mytpi\n" +
            "ihyawsa (77) -> ocztn, eiacrfj\n" +
            "bmiqc (9)\n" +
            "urnnskw (5) -> zdsag, pntxt, hupbun, utxkvw\n" +
            "gbhssic (63) -> smguzlg, mfkfr\n" +
            "enxsaag (71)\n" +
            "ysuhs (82)\n" +
            "lxnqzpf (63)\n" +
            "mjwsaam (311) -> rxfnzf, qxgwwc, flxoy\n" +
            "ghbfbn (86)\n" +
            "iyhde (95)\n" +
            "clqri (96)\n" +
            "eoyji (17)\n" +
            "ciwjgw (95) -> fdztdbr, miymv, qwmvf, mzzmpgt\n" +
            "motddh (44) -> ilmqyc, nmfdw\n" +
            "glbhpwz (58)\n" +
            "lpuutn (161) -> mruemdv, chlofj\n" +
            "pcvfnve (58) -> ssneqxd, arizdt\n" +
            "ucefyhu (53)\n" +
            "hymwone (55)\n" +
            "peoneex (89)\n" +
            "mzzmpgt (1159) -> qnmuc, jtmhpm, gxrmso, gfsmrr\n" +
            "mpesve (308) -> ipbhoko, ddivge, ihvae\n" +
            "zwfoos (8)\n" +
            "pewlflc (37)\n" +
            "qxgwwc (18)\n" +
            "kjibzax (32) -> aejrdpp, qgaohbj, oozzs, tdtiqm\n" +
            "agifkpl (75) -> ucefyhu, jfccr\n" +
            "tkhbr (104) -> ipqome, ohwwaa\n" +
            "wcqwomp (34)\n" +
            "vvxmua (54)\n" +
            "zbcwm (124) -> dlsayg, wuhui\n" +
            "avrjyy (25)\n" +
            "hwhleil (86) -> lxnqzpf, nhgdduv, tpxrpfd\n" +
            "eiacrfj (95)\n" +
            "vdoqb (94)\n" +
            "paokema (52)\n" +
            "mhoofw (95)\n" +
            "diovwj (29) -> hwmegb, bfanbgh, tawdmir, ovqpta, thfdorc\n" +
            "inuxln (11)\n" +
            "snllz (103) -> ykztsh, qmzcp\n" +
            "kuvzyp (29)\n" +
            "ocdsfd (59)\n" +
            "qxwsk (320) -> kmwhbm, rsvtgxk, jkqnf\n" +
            "rmvgczw (1744) -> nuqlt, qvyqnuk, ttdeu\n" +
            "fdpsrru (24) -> ntzgern, kuypvuy\n" +
            "kdgcv (96)\n" +
            "iiropvx (6)\n" +
            "uocco (45)\n" +
            "prkspdn (380) -> lnpng, ghiqkm, wrbrkm\n" +
            "txmua (78) -> zisnz, xqdpgzt, ddjyphd\n" +
            "yswzfg (73)\n" +
            "biydd (170) -> modlepu, ocdsfd\n" +
            "clkduv (60)\n" +
            "zligfrr (84)\n" +
            "mdcnazp (77)\n" +
            "cwluofv (52565) -> feiowiw, zlkaugz, smamg\n" +
            "bavhyuz (7693) -> ijibo, ncpzhu, uqiwwa\n" +
            "zscii (59)\n" +
            "wtfjqnx (24)\n" +
            "ucorqrv (122) -> didkxzb, gozty, ezimk\n" +
            "yxpzp (73)\n" +
            "lqmoz (232) -> qyixlaf, jwenagh, nfrbj, twsvvzh\n" +
            "wqcpszg (99)\n" +
            "yyqvf (220) -> irjtkb, mzsoo, rjbaqwb, mxigb\n" +
            "dbtzr (34)\n" +
            "lshcpil (52)\n" +
            "bcxexao (81)\n" +
            "osydb (930) -> htkhsaa, cfkcj, xteuk\n" +
            "zskvah (84)\n" +
            "rvwaido (283) -> oyxlg, uhbka\n" +
            "wfoili (18)\n" +
            "ptzanxy (113) -> uymxxd, vxcjbsi\n" +
            "arizdt (22)\n" +
            "bbupp (71)\n" +
            "kkdvdz (97) -> baqqex, ikhbgx\n" +
            "jniuewa (39)\n" +
            "mdhduoi (262) -> zfnxo, mxaoa, zfrzbb, kkktvk\n" +
            "tmtqgn (108) -> eztix, gufoi, fkzwy\n" +
            "rsnptie (198) -> lausf, pcvok\n" +
            "htwyil (133) -> ocopf, eddcns\n" +
            "ttegw (22)\n" +
            "ayloijg (188) -> pkwfv, merswry, klrrpn\n" +
            "bylpn (93)\n" +
            "jektxhw (456) -> lsfjnsb, cryxpp, ffvtymw, lncbnc, zbcwm, uufted\n" +
            "hlcre (59) -> efszdn, adnrrap, koacic\n" +
            "rvwtrl (50)\n" +
            "lhbuca (7)\n" +
            "xgydg (163) -> jgvzkp, dgwely\n" +
            "ddivge (5)\n" +
            "xkqsn (29) -> umdspul, japznn\n" +
            "irjtkb (25)\n" +
            "bfhoskf (69)\n" +
            "ttdeu (88) -> hpovffo, ulzbyy\n" +
            "utnrb (538) -> jmrywkk, skrlqr, gbhssic, bxbaua, rwvvlbx, fftxfs\n" +
            "hkyiv (45) -> iyhde, mhoofw\n" +
            "itfjcv (85)\n" +
            "xfvfo (41) -> clqri, fdvpljm, kdgcv, kdwsl\n" +
            "jkqnf (32)\n" +
            "esxiq (174) -> gbziz, thzgmn\n" +
            "nazoxxl (61)\n" +
            "gkvdu (138) -> gicaag, iwexk\n" +
            "hejlvq (346) -> sksjqz, lnrnk, pjegzrx, ovcgnvp, zjiaam, giugg\n" +
            "azzfba (86)\n" +
            "kosdbim (21)\n" +
            "bjfmsgp (81)\n" +
            "cyizx (90) -> kiflapm, fjbvj\n" +
            "viockjp (9)\n" +
            "nsxfciq (215) -> wfekn, cdnagg\n" +
            "cxhms (103) -> iekhet, ljcnfds\n" +
            "nmfdw (47)\n" +
            "mxumuj (12)\n" +
            "tawdmir (32) -> gnamu, ppptkjy, qnsnqy\n" +
            "uhbka (11)\n" +
            "vxzowjy (9) -> gfrqgpm, vfsdg, ltfanj, vvxmua\n" +
            "rvdntii (8)\n" +
            "xoseld (44)\n" +
            "zjiaam (185) -> dbtzr, ortdyy, jbsdgwa, dtqejc\n" +
            "qffvjvy (20)\n" +
            "nhgdduv (63)\n" +
            "ddroz (7)\n" +
            "rghhbxs (58)\n" +
            "mhzvara (240) -> rrizdbi, ebjqfgp, zvdii, vrfkca\n" +
            "vnuwn (45)\n" +
            "cdnagg (48)\n" +
            "xbypzyz (354) -> wdvmq, bcqcl, biydd\n" +
            "flxoy (18)\n" +
            "miymv (47) -> qxwsk, onrbqjg, lgwut, zwzhv, kgsyd\n" +
            "onbize (62) -> djpkm, ucqgh\n" +
            "ymjhqjw (31)\n" +
            "zcibgd (41) -> wrrwam, xoqqhw, izcxju, vqtknqo, anhrt, cswrj, blfzhbb\n" +
            "rgyofuq (16)\n" +
            "zbuar (48)\n" +
            "amsxxe (220) -> plzvkk, ugpaod\n" +
            "blfzhbb (254) -> clkduv, ujdishy\n" +
            "vilcpj (17)\n" +
            "smguzlg (87)\n" +
            "qwmvf (870) -> vxzqqkt, vnantj, qrxvlk\n" +
            "ygqyizj (17)\n" +
            "aafpdpe (94) -> nbxepbg, atwwiy, pwzfoy\n" +
            "ykztsh (89)\n" +
            "bfrlxk (99)\n" +
            "iabco (207) -> fqmjnw, tqjoo\n" +
            "bktiekj (78)\n" +
            "kwkcos (62)\n" +
            "jggsgh (91)\n" +
            "xrvdagj (45)\n" +
            "bspsg (188) -> vzyut, fjueeps, vilcpj\n" +
            "dqaum (33) -> ugjmo, nhpvc\n" +
            "eujhcc (68)\n" +
            "tqfnnek (60) -> gwuxmq, spjtt, rysldfw, aqdba, mpesve, iabco\n" +
            "efszdn (69)\n" +
            "uqzisq (10150) -> emqcvc, blnsa, wcehmi\n" +
            "nynxxb (112)\n" +
            "bkpka (85)\n" +
            "sizgye (12)\n" +
            "wgtdln (35)\n" +
            "yzdou (21)\n" +
            "ortdyy (34)\n" +
            "gogsic (67) -> dfzlxba, gmrktc\n" +
            "fspnagx (63)\n" +
            "laxqy (45) -> nuoxnzw, ehynzhf\n" +
            "sfmvh (72) -> jeylva, uicohi, pxbjhvu, mhzvara, irmjpg, yyqvf\n" +
            "hlctdjw (53)\n" +
            "ynxvst (67)\n" +
            "aiyvkt (70)\n" +
            "rsaruxl (37)\n" +
            "mruemdv (14)\n" +
            "riqqep (21)\n" +
            "mnpow (162) -> eoyji, ayslth, ygqyizj\n" +
            "iclynp (706) -> pcvfnve, gbnmi, cyizx\n" +
            "tsndvs (79) -> rvwaido, npsgry, hlfgoa, criaedy, hohun, extfuy, acxrgal\n" +
            "gmyydys (69)\n" +
            "xbxnqfz (22)\n" +
            "ujudb (77)\n" +
            "vqpzey (1876) -> mjntxj, rupou, xbypzyz, brzgxi, toprb, gqmqt, psyroai\n" +
            "ivlxyu (68)\n" +
            "jeylva (148) -> ibzlhq, rahuwnt\n" +
            "xpormy (97)\n" +
            "zlffm (6)\n" +
            "ulzbyy (44)\n" +
            "trjerm (156) -> yzdou, riqqep, rrlifdx, kosdbim\n" +
            "wbzccs (70) -> pulsq, zskvah\n" +
            "xwfnl (63)\n" +
            "nbxepbg (67)\n" +
            "fatahc (851) -> lpuutn, aemnj, fpbxkb\n" +
            "dzyvcxt (142) -> wrsdsa, zledwkk\n" +
            "asafmi (51)\n" +
            "puxzdr (95)\n" +
            "skykdl (22)\n" +
            "avxjqab (87)\n" +
            "rowhly (47) -> kdcgzbw, fpzba\n" +
            "lqwwuqk (118)\n" +
            "rqpbxi (145) -> kmpdpy, kimfdue\n" +
            "ffcivin (170) -> tlzng, fwfbtcw\n" +
            "zoibnsq (8) -> wzdgidb, imtxkez, uqzisq, mqwdr, vcezyqj, usfiw\n" +
            "xsiklw (38)\n" +
            "ucwwc (78)\n" +
            "opbjmvf (63) -> dbkzlc, sbaeoqo, hazdl, rghhbxs\n" +
            "luzuu (72)\n" +
            "dbkzlc (58)\n" +
            "jgvzkp (74)\n" +
            "kryid (199) -> tenjus, gocurcy\n" +
            "hdahal (81)\n" +
            "fjbvj (6)\n" +
            "ajjyqz (14)\n" +
            "fftxfs (155) -> wqljugh, sjlwjcc\n" +
            "vkkwtq (61232) -> buvuq, diovwj, decklvy\n" +
            "xqqgyu (16) -> fbxcht, uyxpx, ddpua, ayloijg\n" +
            "hohun (305)\n" +
            "cwthmnc (82) -> wmciqdi, tdmpe, wiqjl, gnmudrh, mjwsaam, fgqxuig\n" +
            "veeinqe (13)\n" +
            "gocurcy (74)\n" +
            "uxupys (65)\n" +
            "qjipfjv (58)\n" +
            "wgarj (35)\n" +
            "mceul (202) -> wtfjqnx, rpadzt, grupgc\n" +
            "ohwwaa (26)\n" +
            "fieeic (45)\n" +
            "urawf (88)\n" +
            "fjhgrk (37)\n" +
            "axjaxn (78)\n" +
            "gunuriu (73)\n" +
            "uetcsjx (52)\n" +
            "bwbufij (35) -> lshcpil, vzvwks, paokema, uetcsjx\n" +
            "ppptkjy (97) -> vlwnw, ivksd, etzocl, akvov\n" +
            "agiybx (257) -> dmyxvek, cshts\n" +
            "oziwk (55)\n" +
            "yvedw (88)\n" +
            "gjlsrq (84)\n" +
            "nehvflm (98) -> bhezu, plcqz\n" +
            "rlvxk (53)\n" +
            "eiysj (69)\n" +
            "cynginx (99)\n" +
            "lefse (51) -> jyfkl, peoneex\n" +
            "qfabag (12)\n" +
            "ffvtymw (204) -> hcywt, ttvyvv\n" +
            "ayslth (17)\n" +
            "rladyxh (78)\n" +
            "rnpwyut (170) -> zwfoos, urnycch\n" +
            "mxaoa (32)\n" +
            "ufense (96)\n" +
            "atfrn (35)\n" +
            "qrmkrs (94)\n" +
            "javnv (48) -> atfrn, wgarj\n" +
            "inthrl (80) -> usfag, usvvacs\n" +
            "ljcnfds (68)\n" +
            "rmdui (90)\n" +
            "njiof (743) -> zhqnj, ptzanxy, khykj\n" +
            "zqqxw (17)\n" +
            "zbjhd (12)\n" +
            "kimfdue (42)\n" +
            "vqtknqo (192) -> jqjyspz, dkxedzu\n" +
            "gmrktc (57)\n" +
            "lbroca (46)\n" +
            "kdtiuq (98)\n" +
            "hkhvjq (51)\n" +
            "ohdwce (54)\n" +
            "deusrct (41)\n" +
            "urljsxt (64)\n" +
            "mjesxf (74) -> zbuar, tfpqwb\n" +
            "qdeybxh (51)\n" +
            "kxxqxm (1330) -> xoseld, xpxbgh\n" +
            "ivohs (91) -> hlctdjw, rlvxk, ovhxqx\n" +
            "izcxju (176) -> ybwjsz, bfrlxk\n" +
            "hdwpha (62)\n" +
            "lnrnk (321)\n" +
            "vybrd (42)\n" +
            "kiflapm (6)\n" +
            "icdyrl (39)\n" +
            "fpbxkb (53) -> hxlve, ivlxyu\n" +
            "qnsnqy (310) -> gddaubt, rnnmuk, kjauj\n" +
            "zazlcg (68) -> nkgayde, eiysj, yheomy, xpbro\n" +
            "inriw (99)\n" +
            "jpsejb (26)\n" +
            "gufoi (16)\n" +
            "uicohi (296) -> frqiho, bnnfxqe\n" +
            "rxarqvt (58) -> avrjyy, seeqovh, gmdwhcy, kqkyaey\n" +
            "yadfos (1659) -> ysqwf, zkfwc, bqusn\n" +
            "gozty (13)\n" +
            "ocatfcc (240)\n" +
            "vzcvxt (54)\n" +
            "xochi (195) -> xdohu, lbroca\n" +
            "lddqu (20)\n" +
            "dmyxvek (7)\n" +
            "didkxzb (13)\n" +
            "hpdegx (99)\n" +
            "tlbakdh (7) -> vnoztu, lkozg\n" +
            "vvsgr (332) -> pulpbaj, mjesxf, jvyscs, pctst\n" +
            "avufn (28) -> urljsxt, gurmk\n" +
            "faifx (1004) -> rpywck, bveca, motddh\n" +
            "chlofj (14)\n" +
            "zygpcpd (39)\n" +
            "msjyqjc (45)\n" +
            "dhdhlmq (1181) -> jelwm, mtlapd, nynxxb\n" +
            "knczsg (51)\n" +
            "tenjus (74)\n" +
            "ocwyg (217) -> cjgtcao, tiedml\n" +
            "zzqwyx (62) -> vdoqb, qrmkrs\n" +
            "zdsag (52)\n" +
            "rysldfw (26) -> vyrwv, wqcpszg, mfdhdz\n" +
            "vfsdg (54)\n" +
            "pgigk (86)\n" +
            "tytufo (67)\n" +
            "bqusn (15) -> itfjcv, bkpka\n" +
            "gmdwhcy (25)\n" +
            "wqgini (563) -> xochi, xdfyw, pdfjuw, ujtmmb, dajvk\n" +
            "iynmj (95)\n" +
            "umdspul (71)\n" +
            "qcpdn (31)\n" +
            "gddaubt (49)\n" +
            "colwnyd (141) -> fkurfvl, frkzri\n" +
            "nuqlt (64) -> rfzyvg, dirdh\n" +
            "qgaohbj (81) -> nfefd, ysuhs\n" +
            "sxuafke (250)\n" +
            "efvcgxy (51)\n" +
            "kmpdpy (42)\n" +
            "finmnfn (6)\n" +
            "ohcob (901) -> rggrduc, fbnmdsm, xljdooj\n" +
            "bxbaua (63) -> ynodbxy, tceqj, necuat\n" +
            "lkozg (96)\n" +
            "yzahca (62)\n" +
            "vbypq (88) -> dgdeleb, wgtdln\n" +
            "lkipow (22)\n" +
            "mqruxqs (135) -> eujhcc, giuonbb\n" +
            "nitrl (425)\n" +
            "lnpng (70) -> pkhkeva, lzdzs, bfhoskf, vcdrqps\n" +
            "nhuuo (70)\n" +
            "nkvxvrf (12)\n" +
            "beoww (22)\n" +
            "atnshbm (58) -> taausp, unikoz\n" +
            "ftumhcp (12)\n" +
            "kojrycd (7)\n" +
            "lyoaqtx (184) -> gbkfxbj, xxqaj, txmua\n" +
            "ojwlkyk (23)\n" +
            "untbk (28) -> rqqirx, ftlrjo\n" +
            "bcqcl (278) -> emwqaz, zuruj\n" +
            "tpxrpfd (63)\n" +
            "vxqvj (58)\n" +
            "zfnxo (32)\n" +
            "klrrpn (32)\n" +
            "mlzxc (51) -> qxqzzw, zxozizh\n" +
            "porhb (240)\n" +
            "pelersw (70) -> nhuuo, tlitf\n" +
            "fpzba (75)\n" +
            "qgyngw (121) -> jieqymv, ujudb\n" +
            "negvvn (55)\n" +
            "nhpvc (86)\n" +
            "wbrop (233) -> xnfjw, ynbhq\n" +
            "fgqxuig (275) -> qjknns, kxbclr, uwlpeon\n" +
            "wmciqdi (335) -> opnqtf, ydzdome, gdjawl\n" +
            "laydujd (99)\n" +
            "vuqlmx (932) -> tlbakdh, daaexz, maokl, owqupte\n" +
            "zopkd (22)\n" +
            "azsiin (69)\n" +
            "hgyhzc (311)\n" +
            "nlbnj (13)\n" +
            "ygukv (115) -> wacnhz, kgfeyyg\n" +
            "gxrmso (242)\n" +
            "jfccr (53)\n" +
            "sfyps (20)\n" +
            "deargsb (11)\n" +
            "dqxvm (67)\n" +
            "mqvstxt (134) -> jrtqzng, xgelkov\n" +
            "twsvvzh (25)\n" +
            "qrnztia (145) -> bktiekj, yagjnwy\n" +
            "rnnmuk (49)\n" +
            "qxqzzw (96)\n" +
            "nawhukx (188) -> fkksmck, deargsb\n" +
            "tiwaeim (98)\n" +
            "iwrph (77)\n" +
            "feyiw (20)\n" +
            "akvov (90)\n" +
            "wacnhz (45)\n" +
            "mhpsnlv (93)\n" +
            "yfrcezm (95)\n" +
            "dsqvto (207) -> cpbxfaf, inuxln, bolsi\n" +
            "rhfpsr (55)\n" +
            "gnamu (397) -> pudsuxc, lddqu, qffvjvy\n" +
            "buxlsok (96)\n" +
            "ebjqfgp (20)\n" +
            "vdabfab (73)\n" +
            "memsb (10)\n" +
            "nkotu (29)\n" +
            "rctutb (98)\n" +
            "dlsayg (66)\n" +
            "megix (13)\n" +
            "nkgayde (69)\n" +
            "zfrzbb (32)\n" +
            "whdgk (73)\n" +
            "gnmudrh (331) -> zqqxw, uagtb\n" +
            "chpsgen (2051) -> irckle, epxtul, atnshbm, ostpegy\n" +
            "hyjba (82)\n" +
            "mcsoi (10)\n" +
            "uyiaqu (98) -> gjlsrq, ilrkhhd\n" +
            "bdzqyu (16)\n" +
            "necuat (58)\n" +
            "uymxxd (56)\n" +
            "qdzefux (139) -> hdahal, bjfmsgp\n" +
            "zqega (18)\n" +
            "jelwm (60) -> hkykap, jpsejb\n" +
            "lcpbmc (212)\n" +
            "nuoxnzw (99)\n" +
            "lpogx (65)\n" +
            "maokl (171) -> xdxiemt, ajjyqz\n" +
            "fwfbtcw (35)\n" +
            "ostpegy (60) -> rfvtvw, hzain\n" +
            "uqfiwz (29)\n" +
            "fllyd (54)\n" +
            "ntzgern (67)\n" +
            "wfmql (22)\n" +
            "hugzzwo (45)\n" +
            "oxjor (87)\n" +
            "ncpzhu (83) -> loaex, qoamv, rnfox, xsyepmg\n" +
            "gzwkbw (123) -> efvcgxy, knczsg\n" +
            "usvvacs (80)\n" +
            "cptmwf (75)\n" +
            "xdfyw (104) -> yafzoj, nazoxxl, jnack\n" +
            "ijibo (390) -> xkqsn, colwnyd, unehvbu\n" +
            "nykzm (275)\n" +
            "wxjuvo (56)\n" +
            "obumrhe (275)\n" +
            "fwapzg (12)\n" +
            "uvprg (10) -> qhavltn, yswzfg, fjwfhq\n" +
            "klkyckz (96)\n" +
            "koacic (69)\n" +
            "ihxivjd (902) -> rsnptie, tollmm, mceul, tfjzhz, amsxxe\n" +
            "lausf (38)\n" +
            "sakilbi (57)\n" +
            "gdbapcx (25)\n" +
            "wqljugh (41)\n" +
            "zvdii (20)\n" +
            "wdvmq (81) -> gkyvjtj, azsiin, gmyydys\n" +
            "aemnj (107) -> pvthg, deusrct\n" +
            "smamg (5382) -> hgtwnxu, eksshr, dhdhlmq\n" +
            "tqjoo (58)\n" +
            "abkzl (24)\n" +
            "keesuec (54)\n" +
            "soguk (98)\n" +
            "rnfox (71) -> tytufo, qsoybe\n" +
            "xdohu (46)\n" +
            "bvxxoux (56)\n" +
            "decklvy (1050) -> umnhed, wqgini, tqfnnek\n" +
            "sakwdmk (138) -> xpormy, nhhvktp\n" +
            "ezimk (13)\n" +
            "bhezu (44)\n" +
            "dfzlxba (57)\n" +
            "xypefbg (73)\n" +
            "tfkse (189) -> zbvvekj, kunxx, dqxvm\n" +
            "vcdrqps (69)\n" +
            "fmvna (260) -> sozke, pgarna, abkzl\n" +
            "fyzadg (18)\n" +
            "blhurw (51)\n" +
            "rolegtf (97)\n" +
            "jmrywkk (237)\n" +
            "kqcmyr (301) -> uyhqy, ojwlkyk\n" +
            "hpovffo (44)\n" +
            "mzsoo (25)\n" +
            "tuorl (90)\n" +
            "bzlttq (73) -> hdwpha, yzpxkx\n" +
            "uozna (81)\n" +
            "qznfyv (12)\n" +
            "xpxbgh (44)\n" +
            "abjhq (21)\n" +
            "criaedy (179) -> vybrd, jprgu, tldocc\n" +
            "amqvafo (77)\n" +
            "vckadf (8)\n" +
            "schbl (6)\n" +
            "veujpah (14)\n" +
            "vrqgow (95)\n" +
            "qoamv (165) -> xqfwvbf, gihhi\n" +
            "tlzng (35)\n" +
            "dkxedzu (91)\n" +
            "vcezyqj (8443) -> luxlkxu, dpmsnm, yxfyvw\n" +
            "ipbhoko (5)\n" +
            "krhtjha (34)\n" +
            "xktot (268) -> ewnboyh, sesui\n" +
            "lscpgcs (296) -> fxjooi, qfabag, nkvxvrf, cjyrih\n" +
            "kaursj (1519) -> kjibzax, lyoaqtx, iclynp, jsgvv, fzhncm, vvsgr, ohcob\n" +
            "kswmjp (93)\n" +
            "rpywck (36) -> asafmi, tkneufh\n" +
            "wiqjl (209) -> zygpcpd, vtprv, wrbqnx, jniuewa\n" +
            "wfekn (48)\n" +
            "tvshylj (69)\n" +
            "qkdikgs (21)\n" +
            "plcqz (44)\n" +
            "wccvu (169) -> ukxuw, thnxnda\n" +
            "pdttnl (55)\n" +
            "xkuubv (35)\n" +
            "nhhvktp (97)\n" +
            "qjknns (30)\n" +
            "ltfanj (54)\n" +
            "pulpbaj (28) -> bbupp, jeopi\n" +
            "xcobqnc (65)\n" +
            "aqdba (167) -> slnbq, axjaxn\n" +
            "qadkf (32) -> odqxcfl, kswmjp, bylpn\n" +
            "vrfkca (20)\n" +
            "fdvpljm (96)\n" +
            "qhavltn (73)\n" +
            "niixavl (43) -> siyms, tiwaeim\n" +
            "ieuus (79) -> jenzlcn, kchcxe\n" +
            "mfkfr (87)\n" +
            "etzocl (90)\n" +
            "pgarna (24)\n" +
            "cjgtcao (44)\n" +
            "ctmtk (6)\n" +
            "rrizdbi (20)\n" +
            "bltjqky (8)\n" +
            "ilmqyc (47)\n" +
            "gfrqgpm (54)\n" +
            "titde (75)\n" +
            "jsgvv (368) -> btvxwyj, kdhmnh, ucorqrv, uoxqwap\n" +
            "afdox (41) -> gkcpihl, yvedw, urawf\n" +
            "pntxt (52)\n" +
            "pzwxz (73)\n" +
            "btvxwyj (81) -> xxjcew, fvdsi\n" +
            "frqiho (12)\n" +
            "xkyjdv (44) -> soguk, rctutb\n" +
            "sqzdg (176) -> xrxabjj, zvctpo\n" +
            "ymbtxzb (19) -> qdrbdcf, nhnwqgh\n" +
            "slimx (1283) -> xktot, zazlcg, lscpgcs, zdrbia\n" +
            "eebqhug (83)\n" +
            "azmknz (35)\n" +
            "fbnmdsm (37)\n" +
            "laepzgh (64)\n" +
            "yhmavn (70)\n" +
            "japznn (71)\n" +
            "xsjptl (75)\n" +
            "pctst (154) -> bltjqky, vckadf\n" +
            "khajtz (111) -> yzahca, kwkcos\n" +
            "gbkfxbj (276)\n" +
            "kozlucm (85)\n" +
            "jprgu (42)\n" +
            "rggrduc (37)\n" +
            "oejnh (55)\n" +
            "ugskjw (17)\n" +
            "baqqex (54)\n" +
            "vxzqqkt (419)\n" +
            "mzvyddo (788) -> brtvs, kmzeahn, onbize, cuikvge, lcpbmc, bzhnu, xyvst\n" +
            "eddcns (53)\n" +
            "rfzyvg (56)\n" +
            "ctnckd (10)\n" +
            "kjauj (49)\n" +
            "plzvkk (27)\n" +
            "irmjpg (206) -> cxfgjp, sakilbi\n" +
            "lozmiv (59)\n" +
            "shypdye (118)\n" +
            "pkhkeva (69)\n" +
            "lbrknfc (14)\n" +
            "dtqejc (34)\n" +
            "jholt (9)\n" +
            "kpjzc (93)\n" +
            "xwtls (91)\n" +
            "zhqnj (225)\n" +
            "cjyrih (12)\n" +
            "chcsjda (201)\n" +
            "uagtb (17)\n" +
            "uyxpx (166) -> zscii, lozmiv\n" +
            "vewew (13)\n" +
            "ivksd (90)\n" +
            "yafzoj (61)\n" +
            "ybwjsz (99)\n" +
            "pwzfoy (67)\n" +
            "ghdqy (54)\n" +
            "sjlwjcc (41)\n" +
            "iqwjlgh (6)\n" +
            "gbuhi (243) -> wlqagku, wcqwomp\n" +
            "bpbfd (25)\n" +
            "mxigb (25)\n" +
            "lzdzs (69)\n" +
            "pyavtyz (239)\n" +
            "xdxiemt (14)\n" +
            "cryxpp (31) -> cptmwf, xsjptl, titde\n" +
            "zkfwc (81) -> pvxvvv, fldjrj\n" +
            "ujtmmb (141) -> xypefbg, vdabfab\n" +
            "emqcvc (1138) -> wfoili, zqega, fyzadg\n" +
            "gvupyd (129)\n" +
            "twyuae (37)\n" +
            "ddneaes (40) -> icdcn, gunuriu, whdgk, gkouyvq\n" +
            "vqijdaj (68)\n" +
            "ttvyvv (26)\n";

    @Test
    public void test1() throws Exception {
        BufferedReader reader = new BufferedReader(new StringReader(input));
        String line = null;
        Set<String> s1 = new HashSet<>();
        Set<String> s2 = new HashSet<>();
        while ((line = reader.readLine()) != null) {
            line = line.replaceAll(" \\(\\d+\\)", "");
            String[] t = line.split(" *-> *");
            s1.add(t[0]);
            if (t.length > 1) {
                s2.addAll(Arrays.asList(t[1].split(" *, *")));
            }
        }
        s1.removeAll(s2);
        System.out.println(s1);
    }

    @Test
    public void test2() throws Exception {
        BufferedReader reader = new BufferedReader(new StringReader(input));
        String line = null;
        Map<String, Disc> map = new HashMap<>();
        Map<String, String> rmap = new HashMap<>();
        Pattern p = Pattern.compile("\\((\\d+)\\)");
        while ((line = reader.readLine()) != null) {
            Matcher m = p.matcher(line);
            assertTrue(m.find());
            int weight = Integer.parseInt(m.group(1));
            line = line.replaceAll(" \\(\\d+\\)", "");
            String[] t = line.split(" *-> *");
            map.put(t[0], new Disc(t[0], weight));
            if (t.length > 1) {
                Arrays.asList(t[1].split(" *, *")).forEach(d -> rmap.put(d, t[0]));
            }
        }
        for (String on : rmap.keySet()) {
            map.get(rmap.get(on)).discs.add(map.get(on));
        }
        for (String d : map.keySet()) {
            Map<Integer, Integer> weights = new HashMap<>();
            for (Disc disc : map.get(d).discs) {
                weights.putIfAbsent(disc.weight(), 0);
                weights.put(disc.weight(), weights.get(disc.weight()) + 1);
            }
            if (weights.size() > 1) {
                System.out.println(d + " -> " + weights);
            }
        }
        print(map, "bsfpjtc");
    }

    private void print(Map<String, Disc> map, String x) {
        System.out.println("<n name=\"" + x + "\" own-weight=\"" + map.get(x).weight + "\" w=\"" + map.get(x).weight() + "\">");
        for (Disc d : map.get(x).discs) {
            print(map, d.name);
        }
        System.out.println("</n>");
    }

    private static class Disc {

        public final String name;
        public final int weight;
        public List<Disc> discs = new LinkedList<>();

        public Disc(String name, int weight) {
            this.name = name;
            this.weight = weight;
        }

        public int weight() {
            return weight + discs.stream().mapToInt(Disc::weight).sum();
        }

        @Override
        public String toString() {
            return weight + ": " + weight();
        }
    }

}
