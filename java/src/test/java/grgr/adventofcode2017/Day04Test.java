package grgr.adventofcode2017;

import java.io.BufferedReader;
import java.io.StringReader;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.junit.jupiter.api.Test;

public class Day04Test {

    String input = "bdwdjjo avricm cjbmj ran lmfsom ivsof\n" +
            "mxonybc fndyzzi gmdp gdfyoi inrvhr kpuueel wdpga vkq\n" +
            "bneh ylltsc vhryov lsd hmruxy ebnh pdln vdprrky\n" +
            "fumay zbccai qymavw zwoove hqpd rcxyvy\n" +
            "bcuo khhkkro mpt dxrebym qwum zqp lhmbma esmr qiyomu\n" +
            "qjs giedut mzsubkn rcbugk voxk yrlp rqxfvz kspz vxg zskp\n" +
            "srceh xdwao reshc shecr\n" +
            "dcao isz wwse bbdgn ewsw qkze pwu\n" +
            "lbnvl lviftmr zqiv iadanl fdhrldn dlaani lxy dhfndrl fkoukx\n" +
            "raovmz pdysjsw hqps orsyqw rrwnzcz vrzoam jjljt\n" +
            "wgt gzi icpwp qeefgbe msadakj jbbrma sbj dufuujx zex\n" +
            "cfzx bvyu eswr hafcfy klw bgnhynv qrf aop\n" +
            "rzlq atrzcpb hpl pajjw cdxep ean aptzcrb rzcbapt\n" +
            "xogpf ucc nsukoz umtfbw xfvth ozusnk fopxg ubp iflb\n" +
            "xot nqcdyu kpwix szo cyxv hpmp hwtrc zso nyuqdc aha\n" +
            "mkzf cat tkjprc izxdggf obspan lmlbg bsyspf twox\n" +
            "lfmfrd ooclx tcl clt\n" +
            "dxvnyd nxwojj arutn eyqocj swzao tmh juvpezm\n" +
            "teu eman rlmdmk xkbodv fvrcm zorgy wmwe\n" +
            "hmo fdayx duciqf cgt duciqf\n" +
            "imjnv vfmsha cyrusow xjswoq nclrmjy sjxowq ynjrcml\n" +
            "rwbsay alsi bmzpvw ozq aduui nihwx glwdiz ixmkgfx\n" +
            "vtjzc ntkh zekj qrbkjhn zekj lyfnbg\n" +
            "afaig jqhli oie lhwyduh kqfnraz nfrzaqk mayfg iljqh\n" +
            "inb zum zmu dnl zjxg vrdziq ypdnsvt\n" +
            "uhbzmre mpdxm alkbmsq aopjmkl mqxenry ayvkrf zxvs qkfqva\n" +
            "fimjr ccv cnug crdsv\n" +
            "bqyve lhxdj ydu qbyve vihwjr vyodhc\n" +
            "vmng dyttyf noagpji tdtyfy ewiest ogg\n" +
            "kgscfj klmsv vmksl vmlks\n" +
            "qlvh veo wruft wtm fbrlvjr evo wvwmny dhp bvrlrfj lvt vgzuyyw\n" +
            "mxuro orxmu tivu tjdq ojjvju cdd\n" +
            "kjexme gxbysxp yxrum hinrklv fxehytd qkt tqk umryx nim\n" +
            "kywnux wab tzrft dsaz jgwuw dubarmi fle wjguvr umjp uzncwj mzz\n" +
            "qokwh zrda xywufk tbxhhj eejqaoa hwoqk zer hwt hbjxth xyf hmh\n" +
            "eregs qdx tdequa agrlrg mwwpba qjie yrjvhr txujk\n" +
            "iyot fxwdcb zvwfv vfzwv wvkw ncwbr wdejrr ltcdza\n" +
            "waix eza znmonya ldfghws ialwfvc dey ubsz uhbnh svgekg nonzyam\n" +
            "bryz tfbqn xznfmw xiplgww wwxigpl jxzcgxl rzyb\n" +
            "cqvl rrcoqxs staeuqr hzzow cwv tsvol dio coc ddavii uuojy\n" +
            "txbn qvkkyh gbqnjtq ndpkqr srt bkpqfmm ytycev ypcv bpqmmfk\n" +
            "uqkjmul dour zgq ztango yrrjhrg ufxnmuw\n" +
            "ekxbcv vkxbec xbcevk jiq bar\n" +
            "wczff qdu cwffz hhk wlvyg\n" +
            "zjlconc osgsro dajzo hqih ehml\n" +
            "hnio shccluw cpu ivaby tormn vkef abv vkef ivaby\n" +
            "xgbdeso xiizs omqwy sbtnnt khago evviclw xyu dtvg wsyxfuc humewp\n" +
            "cnzu bia vdyqrf wwb qveum hmh ouupgc owli\n" +
            "pjpmfxa dvd lxgh ndy gwph oebfkqv vtlxdg efl ekj dyn\n" +
            "mvan nmdkc ucyshia mavn ecst poo\n" +
            "oybm pjwm bmyo wovgu xykziaq obmy eiirhqd\n" +
            "xkvomx yxvv oxxpth elh vxvy lhe ycn\n" +
            "okxglw gmaangx gnxaamg yduzrr nzwxtnd rcxcu xjjvno yat cin gaxnamg yss\n" +
            "oicgs rrol zvnbna rrol\n" +
            "abb edpnxuo peoudxn bab ceay\n" +
            "ncpkfz gvwunb fckpzn caafx pkcfzn tsfl\n" +
            "fnrt ymenkpq wodubcm niv nvi ziluu cuowbdm zocg pdakwt mlzxkex nuxqclo\n" +
            "uouxcgl stgua otadr ideannq wizxunv iqsdpj mxddt ldst ucxogul\n" +
            "rbrwyhk wqoz zqwo ikwgexl atpu iza\n" +
            "smo yolp pcahlu muljxkq cbkljmz zlbcmkj zvbmgz eaiv ncv zplifm yplo\n" +
            "ocutdhz zmnaap llgv llzpl loavju guzkfq saay rxyhng cwxzx lcv anrnzs\n" +
            "etyzx tcm upxrtvd imyoiu rdpj fed dmm\n" +
            "gonqa szteh szteh razdqh phyff upf knfqfaf knfqfaf fpsgl kakag\n" +
            "mcju mixskop isrwat lcr nfyi lcr aaevr nfyi pqrbk gnful\n" +
            "xfmr fkmnq fbnhd mxrf litniid xbae frxm zcenf\n" +
            "yuh lzojtj rqsh hyu\n" +
            "vbjgql yeshsuv lokt efqota wpwjfu ykyq rxc fxxh ycqfkk gndts vdf\n" +
            "wnylmr kkuruxm azr xukrkum dohkwx dmdb\n" +
            "bjiyrwf dvf fdv vdf gnokekr\n" +
            "jsaq hcww iayqtu llv gdpxdrd hwlo nosjit wpm lcab fcgwr\n" +
            "fxjp bys nnf xzllckh bys hvojw zcwtgwz wye ccyvjv\n" +
            "grafa hbb ghk wkdpsf ufa uoqmysd\n" +
            "yvacf kssbff iovrm mvrio cfbpb avh zzje\n" +
            "gqd qmsen wkvrfz vhtsa zrwfkv gul zkvwrf\n" +
            "hrbi svaogb aogsvb bgrk hibr jbtkr\n" +
            "ljl ryc mrewrge yky\n" +
            "fcqyymt emk qcmyytf mcfvusb luy qany cbsvumf\n" +
            "oknt mcozuc ccmuoz uoccmz\n" +
            "uziil xobutwf acnof iqgwl diso\n" +
            "sekq fxbtsuv ddnnqg rnemlt dngnqd hhgjfus stxvubf\n" +
            "lajcp qgiw fyz blrlcd pviwv buh wnkk\n" +
            "wolqfk nvpapfc rwcqxfz xobno yzjfz irpj wolqfk wbnwjt\n" +
            "vmabj noiljne hhqf holxkbk swwzx ylgj lnmxy lqodhk abjvm bmsrf\n" +
            "bpnp yrz pjepxxs jlmhuy vihlx zacm inuazhc xsxjepp\n" +
            "tryl kryh eonvaad ucevssk umkxg lqej nswopjj svkeucs bmh stosxxz\n" +
            "cfdwd dmfdrvm ibuhsz nwtgmb pjt dmfdrvm cqdcm fzjjz afa ibuhsz\n" +
            "erwp abn jwx ynmkkj rhgg abn epd atqhs rst rhgg\n" +
            "jtnp cegdsoy gfuvfbg gdmn ahlsc\n" +
            "jgrp diu jrgp onjnml nojmnl vxockc\n" +
            "lakqyuw khq dcpiwt ykwlqua hkq plklx ujbckec hjcvur jnp pvyf\n" +
            "usuvoo jkih ylafyy yhio jureyj\n" +
            "uazisdf cnwlfnf ewodatr woaddkd wbla qmn atdrowe\n" +
            "bnyepaa ntqh xppe ydtsw ppex\n" +
            "yewjwsp jxylmtk ijese ewry ijese kbja nfml zeuwcsh juimz\n" +
            "qbvmf nca zsfreo uurgaiz twe fbqmv ncwi etdcsk atowfp\n" +
            "jeotslx kgdpzp wxlcww pdd dcn ddp\n" +
            "macllv ldl kyluine lbt hbxbr wxcaspp ezwvc qxkeu\n" +
            "ivg gxv zsf ucr uff yrz\n" +
            "tdlwbny bqlrlz tbynwdl lwtbdny\n" +
            "tnekq pdaievs ttwpfh xfm fcaa\n" +
            "zqqhl zbf fbz uqrv bfz ffwavhk foccg\n" +
            "vcw ebqdd cwv eddbq nrmq\n" +
            "hpiusz sizphu xzq sgyehk wgagkv hsygek\n" +
            "vagkxa iou frqdnnr ipcg uxvh vvh eskf katgpiq aqktigp gzvseyi\n" +
            "xkwgd kzfxk pgdy fmtvq ngf rshx zti pamviob ely knz\n" +
            "hwo rteohu qzwoe rotuhe wzb\n" +
            "bsqgg tid dti gtats dit\n" +
            "sjtux djwxv dljwjq xwvjd xnqfvx veqdrtl uxtsj nnkjn wnhilaf unirrp\n" +
            "fruuqjk gtote gooklg bzwhim zfnccmm ezipnf cxwdxa wfu fdca\n" +
            "zcyxb byzxc cxbyz pgcqco ivlxz\n" +
            "wrjh zfdinsf ihw xwosiah hdg xpiabno bilyy azdeczg javuwa\n" +
            "rinlv dcpt qhencba mmb njxw gadc\n" +
            "qwcpua qzyzt cxjsgh kumh byiimas qhsgf qytzz rqqruwp ismyiba xydcxz rwkscqa\n" +
            "xbzefi hltca ibzxfe fkx xizbfe wvaynts\n" +
            "oyuce vzk ouxvj gfh efgbv ubc nyb bxnbhd mtwboe whksy ovmrt\n" +
            "ljrebp tacn bpjler utphw wmfw rcnha\n" +
            "drdnic eyodes rcnidd yseeod\n" +
            "umxmsf kfroz ukhjin awpnnnu ooyyohh tuv rafano jze\n" +
            "bakz lfzpjyg gfkqcgn kzh zwpvk gqfngck\n" +
            "jpaony ojpnya hmro xaaz tovary aaxz iel pbg\n" +
            "swvbgc bbhjp yvrcddd rhj clfu eao afrkegn qvvb yvcx nxjmdo rcvtx\n" +
            "conbjy jeqtri wvujt jeqtri rkhllgw tsdt zowreo qxr qbpragn kuzmplw wvujt\n" +
            "jrpxyp hchljy rkowqb eeaf ltllebb gtksrwx iazx vnsfmc zzrxw hlcjyh\n" +
            "piehb cjdzt eqn kuje rls oaewoz lrqwt lcrrq\n" +
            "hdjowxv uknhlv hluknv pokxg\n" +
            "txiqxfr fyyp pyyf xfxtrqi tvm rtvby cfx trx nwrf kqrxtat alwot\n" +
            "wdaadr stexpow ardawd uejqxc\n" +
            "wwgwjel wwgwjel mtjt wwgwjel\n" +
            "mczx uua lgceb dqru vkcea tcet ruz\n" +
            "jkt yroojr qdrtdu wze ovwz fdmqnr xxsyfd kchytwl hctlkwy gyd\n" +
            "eif irnrce iamhxgh bmis uxye azrwdi sznv yuowb vdlqqxu\n" +
            "dxdjyj hngqwzs yhwku qhsctfe rhbc rchb tqhcfse\n" +
            "fxyxnzs qtxevin rvtxtc iqnxtve\n" +
            "zgbpk mwzxx bgpkz wkpkn\n" +
            "rjiym iub lcyw agbtlb bzhx osv rbtf\n" +
            "emmyu uoflio tinih skpqaj rbor gezbhhv ine mij qlqte uuj ycns\n" +
            "owmwc uhxv pyho ftjh jzsg blqn bszyo bob trbycy mkru\n" +
            "mwgz bbqsmpp fgzs bihhg bbn pjxxexs qrqmt htsxfwo qltqp vqqaxi\n" +
            "lpr wcvy sxjqq ltd rftdgv pdnog ymu\n" +
            "qhcos shuy icdhucu lrikh rwslv yxbgibl rcomhn wakirz\n" +
            "civdmee owlzocl vedecim rogmjnn pix pohcmk dsjm yworm\n" +
            "vzdpxp lvt inufv yofqt omm qfoty qrlseqy amkt kjcvg vgkjc\n" +
            "huhq quhh levzsws sjuun ofgqr cjhp nfxbbft rnt wtbd tbzab\n" +
            "tjftkx xpfcv hvftvhw lpypbjg batrn fhwhtvv uthl arbtn brb sthv\n" +
            "ogr uyuxdco bpjgir edztxv sxtgu jzfmx ihnauz zwegqkr kvkw\n" +
            "mhxthf pervvn gshy jig ezjteq ckkcpy gww\n" +
            "tiljyki rpe prcojy tjkylii moxu\n" +
            "pjsdqc lgqydfd lohck emrtejw axwmo wuuv rfi qzyncmw gjijdfb bljfd xrs\n" +
            "ywjab gynzi relf kziy xmsby izyk ocwoho kqnyh bwayj\n" +
            "bhjlz uonz jhmzuq eiajoos zjnbj tomj bmyv hjlbz fgw jjbnz\n" +
            "kszz xzw xzw prtznyb\n" +
            "ghzk vxhwt thxwv slwpayp qxegmi dawdwo kgzh\n" +
            "ibpcvuf wnuwxu sbf jsj bfjynl cdp jbylnf\n" +
            "epaxr vfhf hvff azepadz pwf sbo pgfzya hslyo rqqj rmklw hwtta\n" +
            "yyolko pwbvxvg xdwl yfje hftep kzzsr kho jeyf yvslxpw kfyv\n" +
            "xmk juyjxy eqno mdwklum reg dgn cirh wmxfyj bnxlgo dlobk\n" +
            "oyv gshqyot jgcqe dsf gyohqst gqgeojo egoogjq dmqpyp\n" +
            "sypianq yss lmhu ulmh itilh ndkda lhiit\n" +
            "qbxxl bxxql ikald nfap qixwbqq\n" +
            "jtqhqty ljysnl nwoj toa bmmyj pal\n" +
            "ahktew sxody nkvsf pbxyt baws wgwfwej bevgzm jus hcvajfy kzrb jwgwewf\n" +
            "jzsb szbj ujngwf nfuuf lfiuxdu uufnf orsy\n" +
            "vgo hto isstyul gau wsmxoqw\n" +
            "uxw itwf epaw hec wape hemol rpwyosc xzxmrll eetz zui kagca\n" +
            "mjncux muv rygdeis rygdeis\n" +
            "qgkqjvf iprzibd fkvqqgj llcrl vbh vlf lllrc zwrunt\n" +
            "dslsa wvoex eqbwj tjem gbx ayn xcan fnacl xggxon gnwjlh\n" +
            "yzosv hcxjiz yvon gcgd\n" +
            "bixpny ecln sda eymt bjiwk\n" +
            "rlcad lrdca adoqfzs rgty mds pwb kmwj\n" +
            "wkai pmryffq rrdmodc wgyx taz yxwg nkap\n" +
            "auynzwc vzg uapdv qkrh\n" +
            "ldmuysp oyu kpn ejbl mfifa bzs hwyn brlw qpzqx uyilao ysdumpl\n" +
            "czoxoj pwnultl wezolbw lyk aonesgb\n" +
            "nqy nhb nle yycp lgtbo ojf dytwyh ufa\n" +
            "rwr eph obg peh pejret prjtee ovgz\n" +
            "vdqf vdqf ycjrg ovzl lelbe vdqf\n" +
            "gvagdqm gvdgqam dmb zaxe nepzwn\n" +
            "emwh bkkbgec qwdgk mhvfsrf wmdfpp ekzuua\n" +
            "mbqw lgkyazt ckyhvnq uladwo owt\n" +
            "qwiwd pbo tkjoqda zapo dygqopv zzdlwfn\n" +
            "qty dhb iinncba ytq kvh idgoevt chx waq\n" +
            "ulffsvk vplsz ulffsvk uxsh cpwgxd ikgcacx nrirke uowcjvn\n" +
            "gknmxr grkxnm fco dilyyj grmxkn\n" +
            "saqxkh uhue nvu fef xsuxq ekyyoc bcaavd\n" +
            "qltwqa vrmpv vhra nof yprauc vkreojm eaq igiy mec\n" +
            "wvheiyg uthy gpvcs nhnjrne mqaejr tfnsly zfbhn entcc nystfl cpq\n" +
            "zxv jzk dwsjgrd gqqxhp xqxu naunwc yeh qzpkz awcnnu aoosa icadax\n" +
            "vpmqmg qmvpgm tqs mvpqmg\n" +
            "inehzu zwxeoy jxia fcyzxc hwikd\n" +
            "bzwnp kamsen ajpn kdls bzh xqcb bzwnp cmjnfa wmgx\n" +
            "hbuhc qgvhxy smzkxh zzebox hbcuh net wyrdppc yvgxqh\n" +
            "oeum oemu iyags xaipdi euom\n" +
            "tqljgoq ghtdhw xhnni lux qltojqg lki zxztda pcqjif acpzvwy\n" +
            "ydijaq kbyjxpu onyd hsfgz geqvbg\n" +
            "rwoih xog dtbzyr ryzbdt tdbyzr\n" +
            "vcdxf zosw pardxfz bmb mscmain lwfc jvq hbszcqh fxomsmm ahnugx\n" +
            "zutsemg pqzil ddv nsstz gmeuzst bedvy xkzzjpw xlqbd\n" +
            "xxf ltnnu yeb hbml agj meovtjr qrul kexerkw xxf\n" +
            "tqrpd hhcx bmdv nlmr pnu pajdtc rpatqi yekedx oeiuew epsshog\n" +
            "ttbfpv plairk toh jagfsg njnqpa tmwh vwqp irtxv\n" +
            "vdky uwc tkkkztp vdky vdky qlcw lza\n" +
            "rzie yundymy pwgx wtwtbg kpiw mewnb liveysj uvsbn\n" +
            "jgfvyny hacg pzra arpz uowswu puzsfu hoe heo vrq naup\n" +
            "hqv vrl uko qgpikho lligvxa wdld qgpikho\n" +
            "whvby yomxwj dieffc jkprinh dsaqy yfrnba woyq yexeb mjn cbszn xeswvvo\n" +
            "wowtgu rciyg rlas bra quyfec ihe thuu asxhscu bsbdpbi ogxosu\n" +
            "vydsaet tvnkjq piedkzj foeiqz zqivt iatsju tjnqvk drauaf vqitz invoz\n" +
            "cppn jqzw zmxr qksuas iifmjg xtkgf cppn cppn jpsd\n" +
            "nkifpsq cxdx bokxhm ebww kghagrp bofhrl grc cheuzyj\n" +
            "ibgrlvm hrcx jjuoh ipmt\n" +
            "hcoqkh fzt rgravb cimauj jxjq blct qhc vjxw pqpg qzp\n" +
            "jycxz xcv czxjy vxc\n" +
            "liljaur cgmg neldxb xfummcq yfhiukd dnqhl iolxn cmewhb\n" +
            "hpvoihj fkwokod txy uuktw vmqqb dpldzh yxmcay cyaxmy xycaym wekr\n" +
            "ccnaf wuxc ecadb vbgpt ccntf sezo skjdkbf fnctc\n" +
            "hqdtwho kdhyman bjtcjvr bwllva ncyffyr\n" +
            "xprn jrrvmj pdw yvexm ewbflbe eapml rvrmjj xmevy rxyzhf\n" +
            "wjcbpy qdgtcp cfjh muww fhg sgfdleo nelpte yucqa aavev\n" +
            "rci vqypsqt xmg rzii\n" +
            "gramh wwprtc ampdhw dajr\n" +
            "ovrm mdyhpbl mdylbph aykz\n" +
            "cbmo fxs nuugu guunu upt ljjuhjw nituh utp kxqc\n" +
            "rhabal rhabal rhabal vah lfrs\n" +
            "nrq qway ftzp rtjcks mbygdtd hsiqbh wypqb rtjcks cllp hsiqbh\n" +
            "ywa anhcf nvd puqkwg molrwck wsctx xvd molrwck\n" +
            "wox jzq jfen wcvus cswvu oxw irg lmu tpj viahm jesic\n" +
            "qenad neqad smlgi ydwzq ppdemvs ucyuf qtunm eoqx jlgv\n" +
            "sucpl nrdwbl ltvetok npbw ozzw hafyay sjmui sjmui jkqlq pyn pbuopx\n" +
            "nxgaiu ybyl meo kgh saqjaz xhbqr otelcyp vkwc\n" +
            "iqrl ldjlwvl ajhrl dnhutr gkknyqs mcvluet fgyu ogiz cxo aiunl orb\n" +
            "psd cyq xpoyqny yqc kozqh vonfd uhozwz pds hcpw\n" +
            "tvaxder tulwmw qiw avddbmh irog vynjzcc refx efxr emnvk\n" +
            "myjx npqk whm egw kpy igrrohg ukglx ldnuqw caqg ynx fckhnsh\n" +
            "dafv bkdoqg zcqvbco xgikoac cvbqczo\n" +
            "rtzhpwk ukuyp bayhzp upkuy ahbpyz\n" +
            "oarcuv pnlkxvw fqdkj hwzsz nauwl lpufibz vzfbgc unkluxy rwh xuknuyl\n" +
            "vxhsaj ppdxw qrswqtu ulwv uqtqwsr ppxwd\n" +
            "cww cww cww scu\n" +
            "wiiikwa bfpewt zbgxfkl iqpk tpbwfe aazdcxj ipqk icggn fwn fjr\n" +
            "net ovxuwpz yvzmzd yvzmzd\n" +
            "xgar czuhp vuhisaq fgrqxy evvrtf mnmar lsk\n" +
            "hld mxuedug itswju vmmejqx snzslqj toe bbmugph mgubhpb mowj nrjnzu\n" +
            "qbz ouhye hsldmp lcf hyhlrb ewvle zko\n" +
            "cke mupaq quapm eck\n" +
            "owu zdt lales tzd apjjo fhpx bmuktbw dvehpz\n" +
            "libvl zxypk azazc vtsom ohdzycb\n" +
            "kiowxnc scxygrf ckxnwio ycxsrgf\n" +
            "vcjj fqz lfawfx mps zhv qykch vhz psu zud spu fnpvkx\n" +
            "scfvum fuktgk tua ieosetl wwmjtt exnsw wwmttj plvd pfb kku pdbom\n" +
            "wkfw snukd wkfw gyaojdf bjw htagy cdsp\n" +
            "beh gatqxcu ibrooxr ssww orrioxb eenkqz\n" +
            "jlv affah mtbemf tylh aafhf\n" +
            "zqfajd uwzrw csouuip qzadjf\n" +
            "gsnlrw tcel hha tfbzrp ild aenqa\n" +
            "iirfxef kdux yvj vbzgj\n" +
            "ibx pfll rgkp nancij llpf xib gbkfy\n" +
            "uvw kkbavj pznsnk okigtxl ogitxkl eobbs xhaz wroabn ltogxki\n" +
            "bivdf lotvmoh vrb kpaeeue tdab qhukcb qmy kuqf kesu\n" +
            "egs hbsfeu esg twxko uib\n" +
            "ocraimu qilp ijmx eco nhevqp juxf ksejr bcqqau uhpt\n" +
            "pyx jmpglf juokd dxszjw cml vcjge pfg\n" +
            "gxwrt btmimse dkpbha idmz mtignka ngakmti\n" +
            "dpjhm jyalra hukf imocr lkgt rqywn quhe fukh\n" +
            "nbau xyc bdh yni xaawxm cyx xwaaxm akx gyodqe htbifc\n" +
            "bywdxe bfrp rvb rndl onal jghiwb nuta aint qlciwcx\n" +
            "fpic yrqce land soxhci qzc zoebsq hcdohcc fzhcl iyxb dqinum hchdcoc\n" +
            "zok ghgp zok lmk\n" +
            "ozfz zofz dkdekzb sqc\n" +
            "gfti zuqvg cexmtyl qwuqnj stepb erduqhy cuoizcs qudyreh kqvfdd guzqv\n" +
            "jrugz jzugr lmqu jgihgo hjfbz duxkn unxkd\n" +
            "ckiys dbqmi ckiys ckiys\n" +
            "iylp uvvdp pluifaa djo\n" +
            "esxec rwvel djxppqf jymwt ilm aiz upn aiz wrfefwi rwvel\n" +
            "nitgjr pokxuy puhdwg qtxpb veylp zqvzkbd lrvpcgu zuy rnigjt ibci\n" +
            "jboyzq ogcldr hlon ywav jqqtz qjzqt vyaw cok\n" +
            "aqdw jxn hqknh azbylg\n" +
            "jya qpxtmsj hqrtsgg qjtpxsm\n" +
            "pofcs sxw dlvru dlvur swx\n" +
            "yphvvb qqyyfsp sjkbff dqyerxe jxzes oof\n" +
            "pwbya txk bbwsj ywgimd kmdpc bawpy lbnt\n" +
            "bkbazff ldmaq tyfl acqurpy ndnrp\n" +
            "asw ctiv mnxzyc weeuwb gsn bzk irbyhxl cgqomj izy zbk\n" +
            "yrxcrbt bcrryxt pofe wwzl\n" +
            "vuaqez kbtuyai vuaqez dxqud uvo gmhtg dxqud\n" +
            "tpzs gqdxpxo zzpgta uurjx xpqxodg\n" +
            "cil lsv vznqw vro zqzvjhm jhgauzw uxnwk lci zpgpu frjvyzo tsv\n" +
            "zfvcuim gwn gnw dxfppok\n" +
            "btb goof iwadca aac tbb jha uvzi\n" +
            "qah ned ipmure kyta ffhrwe njz paq kaag xmlui\n" +
            "rkmw vrblwyy gpax hxsf zpbza gypuwf jbib ypcjwd vrlybyw\n" +
            "yfjljn uxpvg huik jsnah nkhsg yfjljn lqzsz\n" +
            "hagjlqx agnax jqalxgh rvjgtc mjrmph azznzcq gxajlqh\n" +
            "ipki bhoabp rmiyl dmjyxl zzsmap aju\n" +
            "tyjrr rigrf ciq qic avmwu jtr wpq\n" +
            "vuf cosgytm toycgms ufv qzpcbrs\n" +
            "epzgxr lydrsj ezxrpg expzgr\n" +
            "ecm prj kmak makk jpr\n" +
            "ccwyq txy okj matxa socoa\n" +
            "zrjphq gigayv ywkfmru yrwukmf fxjjrha gqkxx zhjy tisutx kufrywm izjfj igg\n" +
            "lfhgsro gsroflh wrpo lofhgsr\n" +
            "kgkgj wkhnab ubrjaoa ubrjaoa ubrjaoa ggdgh\n" +
            "hztutpn epnqmz ffcroq mnqpez niibpn kdloak xjui ozttj lyzsc pzgq inpnib\n" +
            "kruz sjqp mmd hhdxjgc mauouma asevvo upjwqi hxcgjhd etqzagp\n" +
            "zylf qime cho oraid svytv gqrjufv mker cho vnkyiin tjms\n" +
            "dotjul qyv hnh cibtg gdpauyx wzp\n" +
            "fabtira ejxoeor cqyethv ndjrq hnxn joq otng lrr csytrub\n" +
            "txhgepd fwdaanm nawdamf pxine qqrn pronw exnip qwkimt rvy\n" +
            "kuxzhi jln urzxtw rzu ebsuylm tscru qwlhfgq nnu nuchvz vuht\n" +
            "cqgu camlr umkltcf stx izp rtdwxff wkfvs\n" +
            "jhje cxix lefcrsu nebv idfzhic xqri xkft\n" +
            "utzxb znb ietupd uqgbhje aobip oawjwm hetyan uqtqv hpwzyri kwxyu\n" +
            "jvzvbt xuyvp aegdkb srbw bzabpf lyfriez cruyfu\n" +
            "nhi nih aeb ihn\n" +
            "hcf zypt djcm pkjx pvhh\n" +
            "rhvxcfk exydvk ids hybme hnk yfchvs mjbo meocn\n" +
            "rpboxr rxoprb hdzje zhedj\n" +
            "ziildbo apzvatr vsv isndq ebxyy ntm tdttg wkvdh qnids vkdhw xxolip\n" +
            "ywu uyw ipcjz pjzci xjn kvgk vsocprw\n" +
            "euzo njlpv ndrlhi drlnhi ivmjkb fjrtxta skvgmrd\n" +
            "gbyvj dkck gevpfvb lhadhx rgjcdn yraxh bdk oen vqryd bkr\n" +
            "vgkp hncttxb wgxh gdyjo bbdfzvc xhgw rznzgda yxrrlo gxhw\n" +
            "ifjlb fpecyic svhjp ilmj oxgr svhaf\n" +
            "vbqky lhccj xtmm xzjyykn oqmdq qywir bswly\n" +
            "euxxziv totzer vsxfx leo djho uoeaz edaig fbu lumbi\n" +
            "ooqtwq pvo kid vpo jxin bod btqc fbyuz\n" +
            "jhabi mronu htqqyz umjcbv sgnbp wyn cetmt pcjf\n" +
            "tnrkcyl dduuhxh rylkctn pwj rtynkcl mzzfomr\n" +
            "rxx ldqffi ulappk nltawbn tplhb kyb cqyi\n" +
            "vzkw gviooah vxh xeae ohvcad oaiwcj dkx\n" +
            "sdofdjt hcifv dqws sia mlwm vfich kavh myzue roops mzuye\n" +
            "uxs nlbmjp nlbmjp tlaxa tlaxa\n" +
            "ynnisp twx xtw jgkc yinpns\n" +
            "kumorsm wav xhx bpvz clqc ffmadzl ndny ymslo lobv\n" +
            "ljzabj tqhves mezh pwn wue dwfqq lynvtt boeknvi xqbd pkud tzlanis\n" +
            "lgq qiikzl oihnsr pivtjmu qhic yvmeebg rxu qgl yuxnqse dvu faxqez\n" +
            "ldk mlwja vmdqr yzlxiua amlubt ejmzfx nonm zhkxbn gaqbnqq\n" +
            "ttc ctt kneknx smtnaft abljip tct\n" +
            "uybhbiw zwojzlm cfxoopp abulenj znz zzn opllzmm yufk witwxzp\n" +
            "qvkybwi rdbxb qiuizmo fqgne jgot jxz dqhapn\n" +
            "vzinf ehaley amnk laheye invfz\n" +
            "pedakl ivld agzyhr wmzba tzzzg bazwm wjwgux thrnxkn\n" +
            "cmyhae nwfs nfsw kmh pxkaffq\n" +
            "vdf szupev tyunp qiiu deevxmy wozvtt nelnr kgdexy gparqj hajavz biizn\n" +
            "pwspk skpwp ontbjee pkspw cfbj\n" +
            "ihsmh djxtak wkzllao oyr djxtak prc\n" +
            "uhvihqq jrgf hdfek pdrfpt tghz gthz awae wcygi wujti svq fhedk\n" +
            "gnfhsj odqlt netmsul rviio nkzw nkzw\n" +
            "xyvc clxw cyxv lxcw\n" +
            "duegck pkviu npwsp zdx wpvn dmxgnv ixv fybs xteru\n" +
            "vih kgk hads boaddu daiwo hozoufv nef vtcplc isiw\n" +
            "tzqoo dqlgvno jzlay sywx ecej addt ecej addt mnfcu\n" +
            "ymgmby zegudpx ipsjai ger wcwjw brzebb\n" +
            "eqekxlx itra xekelxq exqkexl\n" +
            "rciu ojaa ircu nxjga puvmwou remgu\n" +
            "sltth pprimb slnxopq avtir hvpv ppww fhfap wisn kzs jcuuuuf\n" +
            "xbppc ydpbq zhjh oym iljzvk vsb\n" +
            "ueye shtps uccehi ccheiu dqm yeeu\n" +
            "gwywf lcpv qza qza gzuovj jfzffyh oybfxqv\n" +
            "aawi ynsvdco azdoz cqr tnyquq xlyvbx eca kcalpes\n" +
            "zumgzhy rou kguqa vubw bwgd qprxcg etnbev nqmi\n" +
            "fyd tuoz uwclqn cgl lrpkf irz dizv nxze clg jghx jbpt\n" +
            "kwuanos eorjr tcahp kwuanos cyrpfji zxayggd kwuanos jkqt qqvbork lizk\n" +
            "vtu ovje vhg ovje vtu zcy hrhtr puawfgv\n" +
            "bliz exp wot svxv epx\n" +
            "jiqgxwj yips hjsatc jgsrno msfp vxvbt bba bqmw xjgpgog\n" +
            "vpvypp ggwp wggp gndp hedpse afji hcqgof\n" +
            "hxueubt hiynoa qqzaj ohb qway\n" +
            "akq nfnes sdrlza nfnes weq\n" +
            "udxpdpx gctuv llhxuow rqtetm hdbnpte oebapv civy oeobu ftgivd pykj\n" +
            "pbgbvn jgmr xrz dfn gosjobw ndf\n" +
            "gnf dtbsnc fwcmml tscdnb fgn qgadusl eifpk\n" +
            "vmnv yuxrup qcphi tanc tnca kjrv cphqi\n" +
            "hclggs sghglc fgplp odn pfglp emkrulf whwtmbs qnuyg\n" +
            "wcxtr ani ain sha hsa zxbkf bzxokat qezo ljqxi xqcwfmd dxo\n" +
            "waiq smpbu dbyka uibxjrg nze wiqa rfpts ddjsjv jqqjez bpusm\n" +
            "lpcxf vsbj owjwc tuqj vkrgrh jsjdepv oil lxrjox frsxsi clr\n" +
            "vzunp prwk nnd rfs vpuzn\n" +
            "pqpqv lvsk sqxf nhobsm hakbn ywj\n" +
            "xxu uxx szqnmi lnwtmx\n" +
            "akq nmlw fupwsth jduvhva\n" +
            "nac wwlxqck hpbce vxxqa fyp xvxqa kxwclqw yvlmv bfwi\n" +
            "pzxjbj nvwv mdooiez vvftp enjrsck iypu uhru fpx omtd\n" +
            "llxgp qwf pwaj cuhb scloot hbcu jgp vjw ooclst\n" +
            "sisd akawvzd wvdzkaa gyoij ikt eeeosb jiwiup\n" +
            "tche vxj sbctqv jvx gosur usgor ibo yqxo qqgd zspl\n" +
            "cidd welisl fxblxqk qxbklfx fbdoqcz glhq iylodvz zvds ghlq\n" +
            "cnsa hrxst mrnkqtj bptq jmi cpbcofs kveyeur uzmga modphm rtx kntqjrm\n" +
            "dvyup usfaq rtghoec bvcos fqsua zohwwg\n" +
            "onf vncybi dlaxni oqyqqkn\n" +
            "okfwa qyyx ebnv llql nphq etdt ytgivlo jwgwz kiob\n" +
            "ann vqnqvpx wth lpwid bjvzw xpwqxcj azg ioeyzzp onwf\n" +
            "smy epzomx xep yid zctvrfj astdj cfg fgc eriuxt\n" +
            "rljqgin wzobzrh cuwtx vcsbx tmg tuysq vxipgho\n" +
            "ewp rsrnsj wgeyin lrji ddgt utol xxwut fjiwopa\n" +
            "upu ftvqbk tfkvbq fdwga rmu puu hbiasjw\n" +
            "cfl lmqkb lfc wbtlfi uqsjs ejgmphi tbliwf nzcela gzb\n" +
            "zop unwmiu acull mkwh hvruknw rfk mmhaz iqmenq fifino\n" +
            "iczua bjut tlgf zicau jtbu\n" +
            "mtka ipd mdifj kps\n" +
            "irqkysw xfsjl tedx yckkbx iktxb sqxn pbfvubv uudzppz\n" +
            "mdrn cihat wcext kufs awwtjok pfjg\n" +
            "wdevt tyo zzbp pqlqq wdevt\n" +
            "yhatqkv ntuhw tdfd buxazh xbcsv bas gkv rbzi tddf jbj bsa\n" +
            "malip hiiy qezz yhii wlfojre\n" +
            "zqnfll bssveq lprwbep bhqml tztbt\n" +
            "npnxotu yupdytb jptqo klfydfe fpucmfq svxcqr unopxnt\n" +
            "gdpz gwj iytiohu efk ctjzf asade abhotq brmhu tbtdur zzksbh\n" +
            "kxft klzslf tjdzciy lzslkf\n" +
            "ejei ezmemvg xlt zte tbwhz dgnfpao zotck wus uaz gbwbb\n" +
            "dgednf vypmbs eiytot empfmny\n" +
            "uopmui uehue wdvzt adpfcif mutl ifaztka vydi xumtz orstno\n" +
            "dleero olxiq gxnlfm nfmxlg wloeavr olhrwg hrjd yicj ymyeex qav gxyjgfq\n" +
            "hevj rqcne zycgb qgqtn rqcne ptfvu yyyu zlm hevj\n" +
            "zrkhuh sttnkt hkuzhr vqtu\n" +
            "ppsfm kcao qjq dgadglx cxaawjn pbucfu fed qgioarc dfe ricoaqg\n" +
            "vmawf oktunea zraoir gkt zraoir jcvkqoq\n" +
            "mqgml ecawug ugwace szwul iwbmooj owmiojb\n" +
            "auggaw cypcuw npci vuyxijd pofswjx vdkrgx xylk rom ksj\n" +
            "qmwx jgsrdj ikva xzxw avik\n" +
            "zzhcqu rbg pywjdn wyndpj zchuqz\n" +
            "wzd wqycftu yldezp zovuy oydia hovewe\n" +
            "kfid qkkk thak qhbf rvzlzvu uuxh pbj hkat gow oeqcw knqqzha\n" +
            "sua itv hfpg bdqye bznlrk hfpg bdqye kvir kaai ggtz jqn\n" +
            "ulggl guitamm tkpckso fupacz otxtqpd jxnqc\n" +
            "ueesb ndyik vjftz jgqqv nrcf\n" +
            "krh dqpmsw fybzynl zhjbvkw exefc rhs neq ldprb bhhvxm pjwirun\n" +
            "ymavl qwxr yavml wagwc ekokrpq zewppw iumcgin cxdvwx\n" +
            "wwdukav kuawvwd kowv dkwvuwa\n" +
            "eazot bil tzu vdwwbm fvauwrq\n" +
            "esq tixokph yspf ztoxfut lgzush pwv swh pwv auqhuu tixokph\n" +
            "pdbeyxi poio mugfkb brwbbx aao uszw fokjeb uswz\n" +
            "sbs ryjr ptispi tvnhu htunv vthnu\n" +
            "czjmg hbdjhvi jrkoy fpgwc syafy aar kvnq eaecsb wqzpx\n" +
            "twtp dvl uvyje qtlzj dsvyr qpjnj eyoigx bhgpccy gwn dtuf\n" +
            "mxit xunctu vbyks wmqc jriuupl ybvks uncutx nsoxwrb ykt prc\n" +
            "yye mgf uhc irowpc dsdv iwaxod ftavlj dxzp tcch tcch mefz\n" +
            "rxe xwrrgl xwrrgl duu rxe xbbgoe\n" +
            "ucsz akswcd ojrmqq cox hgfh lxwu ltnnf cenikcp\n" +
            "opjhdp svwezr svwezr opjhdp\n" +
            "qojlkl ircxqnt utfmdg fcvr vehkcvt ufmzcpv xwlh ddavv xel bwlz fii\n" +
            "rzkayeh iursm zhily hdnq fqydfvt uwoy hptpiqu tdqy bgr xdr\n" +
            "ymruz umzry hbltwya jhwhzk flh tahylbw bdbaimb qscbp ntkuf\n" +
            "uxpato owsqyao vaog oenomkc usrmnc epua vzkppls\n" +
            "qxqczbk qyguz alawj xgjawtw wxtjgwa snfcdmz\n" +
            "fjfgos rmpd mgs vbk dlls jkljao eoovdfb ucdvaoq qmjmqku ney porr\n" +
            "nmcrqz zcoxpk dlnzksd ymh zyg spxss ruyk bychq gsgv eusiuid mnrqcz\n" +
            "jbzadnx lzl sdamer okoico frqisrm lxet agriw\n" +
            "xceoqr qai vahc jjzifsn exg\n" +
            "igjpn wfy ukn aag quro wklsq cjq bgtjrdz gmub wyhh\n" +
            "fzlwnm mygfn vkzwvw zvhsex gfki\n" +
            "ijvzgai ebmeq wssfmbq uguh sfuutm nwkgmex dxael liakdxs rnf sky yowpxc\n" +
            "bjzkyjh fced nji esowk qxsubsk qgtts\n" +
            "nkdgo bbjfq fgnxnhd gfjchl jetdb xubsgj eiju ldlm oxsx znft bbqfj\n" +
            "xovcnob pxfe pmstes yzkdm iqlvha nmcziix fexp ivqalh rxecqps\n" +
            "xpyew xudfud wwqe qhfjlcu epv fnrbgyv ihli qngtx yjlfg ozqbzn esp\n" +
            "timl gcohx vqzic gzm shwlkkv icqzv urchuc\n" +
            "xpqq gaqzwo cci dowahsr gaqzwo\n" +
            "jjsagdl umbpxre kyre zvaryft tmw pxpnjy\n" +
            "aqovcz nunq nnuq xjrvvh autjmit jiatumt\n" +
            "elg lps lge zjjot hwz tmqrup xaxxmo zlbzp uftd fukdad kvpymsm\n" +
            "iokwzal ywti zbdmzbu lprywe wbgbwza ypogbga kzliwao wstqi eqm keaeaj gbabwwz\n" +
            "lwfpk mhufe eddzgd ljxyqy vhzkct uemhf\n" +
            "lwqil fzugdo faq feppo usl llwqi\n" +
            "nje hthr ropq qvcepu bexszfj avmzjvv zajmvvv fhcd xnc cnx qnuaux\n" +
            "kvksn dphbyz nsx wrcc ccrw\n" +
            "nzpa pzzunfv ygzjy gxrrtcj hrt trh pwxpg yifgjmo fnupzzv wbzx\n" +
            "aepti rbojui ypvhe ubojri tcema aan dntkw qjx bfvmyos tcm hvoqytn\n" +
            "qpwq exu jvsiwj gsw avr vbemldy\n" +
            "xsbzpf xbzyvx xax sxh vpxt gccy xxa zhgbwoa hwwxoky fhvdxfc pvtx\n" +
            "pnsa ovtjolz tyutl eyjjzt jvtoolz owbypvr tytlu ewtzgec\n" +
            "cyg dwwk eihsp aeuk bbnay aluwyz hdmv uaek mwt ihpse wjhnkeg\n" +
            "fhzx vjetz vjub tejvz\n" +
            "ewwyb jidhu pyvyenn igtnyd tiwr akwkkbi myz xxjwb jjrdeg\n" +
            "jbkuw kwir rkiw ubwkj\n" +
            "bltffuw lftwufb hhsh wfbtulf nrxaa rlszi toijxnz czlci\n" +
            "bqrm pga zgblgcw pgwhhn lcgzwbg bcgzlgw yqb\n" +
            "mhjj vjoa gnjlc kclcr ito ofksy giavy fpqeioj\n" +
            "bkiqmif izidbui sttxxi bswhkxp sduuw\n" +
            "mjgnvw mjgwnv ojzyuv gvj\n" +
            "qxn kkhc whd fgwk auzugg augzgu kqfov wfgk\n" +
            "spdxbnu xpfofsb bpfsoxf ahjywql spbxoff\n" +
            "bwqxhlm wbqlxmh kqgpl fyzgf guhkvgx ovk qhmp gnrmu wvd wedj\n" +
            "vvwf hcnc vvwsngj qedzoxm hcnc qedzoxm kjthdi cbwqep qtvu\n" +
            "gio iqklmro noqablo bab jiqc rwebyg rqkloim wzmgs uunl amqs iwj\n" +
            "snxj szobqt zcgvwv wiyqknu\n" +
            "uto jteikwd cew gqsks hmvjtcy sach\n" +
            "zpgl qnkoex amhufmr figns upv xezrl rjleak nwrna\n" +
            "pzkvrdz dtonazj gtr gfxucuf lstjl lsjtl rgkope kzpdzrv lyptn zfxjys ttk\n" +
            "ddxgm lumlgki jhv doft kok swy ckds swy ddxgm lbfbdv\n" +
            "qfs rcufzgz iaiqw qfs qfs\n" +
            "nvkbo sgv mquwb ritpye nbkov poex hraorm qrrr qdt qefl\n" +
            "irxannd fiud ehyb ggx plqg pvvn uuptop tcvbm abuf bcfnmw\n" +
            "qwya ukblz epmbfr vmlon yqwa\n" +
            "hlo mmv vmm mvm\n" +
            "svzpxun yugbbe sbbpxs dmy xspbbs zhpovyf fyovhzp cpbt pke\n" +
            "zgk gft zybs zrgcoo ypu bue htgo\n" +
            "xnesq srsx pkzaoh cfqzugh\n" +
            "lntd nvxetbv clykjpd svmibpx evxtvnb yldkpjc\n" +
            "jsqq tzwak hephg eqwczd ioisa yim tmdifn mceip\n" +
            "kuwqz wzkqu zwchmj lfec uexne iztp llityt\n" +
            "kvamkpc pvbryqh ion cwizjde gln kcpvmak pzzlw gnl\n" +
            "ydeqf bfaab sydqhbp smsxdjr pynrs cqymt\n" +
            "onb eiab bno nob\n" +
            "mqslq scnelxv hyllrf scnelxv mqslq wmnbk\n" +
            "pttu kubby lgop bbyuk gsk skg ikktlbb inbyvz\n" +
            "xznvl zwtdj vbxdyd clhw\n" +
            "hgy zudelp ickc drfjgn iyws xhc\n" +
            "zzv wik iorhat qkb kjb lykdz vrce yjsjwj\n" +
            "gyw xzgbi efus uuy\n" +
            "hwcy ujdun bjjuvd jbdvju onnk xeyy mmp onkn qyzl\n" +
            "jwfm ptjwrbl hhuv uolz adyweh qpj wxyogp igvnojq jmfw pqs fsnirby\n";

    @Test
    public void test1() throws Exception {
        BufferedReader reader = new BufferedReader(new StringReader(input));
        String line = null;
        int validCount = 0;
        while ((line = reader.readLine()) != null) {
            String[] tab = line.split("\\s+");
            List<String> l = Arrays.asList(tab);
            if ((new HashSet<>(l).size() == l.size())) {
                validCount++;
            }
        }
        System.out.println("C: " + validCount);
    }

    @Test
    public void test2() throws Exception {
        BufferedReader reader = new BufferedReader(new StringReader(input));
        String line = null;
        int validCount = 0;
        while ((line = reader.readLine()) != null) {
            String[] tab = line.split("\\s+");
            Set<String> s = new HashSet<>();
            Arrays.stream(tab).map(p -> {
                char[] cc = p.toCharArray();
                Arrays.sort(cc);
                return new String(cc);
            }).forEach(s::add);
            if (s.size() == tab.length) {
                validCount++;
            }
        }
        System.out.println("C: " + validCount);
    }

}
