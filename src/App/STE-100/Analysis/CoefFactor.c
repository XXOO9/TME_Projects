﻿/*
* Copyright(c)2018,成都泰盟软件有限公司
* All rights reserved.
*
* 文件名称:CoefFactor.c
* 文件标识:
* 摘要: 文件实现人体成分系数的定义
*/

/*当前版本:1.0.0
*作者:廖建伟,修改日期:2021.3.8
*/
#include "CoefFactor.h"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////1.宏定义以及枚举/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*每个测量参数的系数顺序:
身高+性别+年龄+体重+阻抗系数(30)+常数
k0*height + k1*gender + k2*age + k3*weight + k4*z1 + ....+k33*z30 + b

z1~z30顺序
z1~z6   1k 5k 50k 250k 500k 1M-------RA
z7~z12  1k 5k 50k 250k 500k 1M-------LA
z13~z18 1k 5k 50k 250k 500k 1M-------TT
z19~z24 1k 5k 50k 250k 500k 1M-------RL
z24~z30 1k 5k 50k 250k 500k 1M-------LL
*/
////////////////////////////////////////////////////////////////////////////////
///////////////////////////2.全局变量的定义/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const double g_CoefFactorCode[ROW_NUM][COL_NUM] = /* 存储人体成分系数信息在固定地址 */
{
{-0.0124420427227548, -0.0189311077270207, 0.0001547036852463, 0.0228174789607237, 0.0005608661603133, 0.0038075719433196, -0.0007297321940186, -0.0016912297645321, 0.0010834154482199, -0.0027200665125150, -0.0136332986813180, 0.0178782342492404, 0.0181177683842534, -0.0110566288478806, -0.0092372252051133, 0.0006018301299898, 0.0005868370181759, -0.0008782788350638, 0.0001387386214874, 0.0001303749608440, 0.0002213263939244, 0.0000460008728340, -0.0046245552525616, -0.0072318425621470, 0.0067153838054788, 0.0285816339452157, -0.0270829584977940, 0.0038541022455778, 0.0031826441991649, -0.0048380257543614, -0.0089534016983715, -0.0002912588280305, 0.0032521747092297, 0.0061148067632070, 3.8641723173899813},
{-0.0058594836659327, 0.0144386981699408, -0.0001544386863180, 0.0136303505474796, -0.0014619784567166, -0.0052294233813787, 0.0057796265781276, 0.0011463614369830, -0.0020037792624726, 0.0010505879373134, 0.0107833398099911, -0.0084568294966959, -0.0076083179334148, 0.0103177289045899, -0.0039135671981288, 0.0003553304942855, -0.0001048159504681, 0.0001054070530289, 0.0001058594827718, -0.0000398716077991, -0.0000618679248924, -0.0000222401258636, 0.0025449607695135, 0.0042639311717159, -0.0062816311994410, -0.0063068597216269, 0.0066120800043862, -0.0011439849444069, -0.0013343529023618, -0.0014221327525769, 0.0087062267921559, -0.0014694367937285, -0.0049442146658704, -0.0002660492340180, 1.3460104606819649},
{-0.0014202683195833, 0.0026120310624505, 0.0000970332913487, 0.0047720388569896, 0.0009916466384474, -0.0010843246973842, -0.0008980574048087, 0.0005319009458510, 0.0010490386319933, -0.0008336988216088, 0.0016343880559627, -0.0015541765771247, -0.0005501514454167, 0.0004890915031714, -0.0008913034709734, 0.0008767678975692, -0.0000882258371772, 0.0000787695701069, 0.0000849590265390, -0.0000764267887726, 0.0000325545645595, -0.0000139849992787, 0.0001714346314584, 0.0009120610396151, -0.0017260423654846, 0.0033119717729201, -0.0023123005066363, -0.0002764378059667, 0.0006647857067810, -0.0011583094008544, 0.0003278282346271, -0.0001014115350578, -0.0008843756667411, 0.0011334531318667, 0.5412980026311571},
{-0.3222776031072090, -1.7073823437198883, 0.0739840825652215, 0.9203738388754175, 0.5619150262153482, -0.3279064931431491, -0.3693590308117143, -0.3932293354173946, 0.8541529674162509, -0.4184745880350683, -0.1906292991860336, -0.5664653280667742, 1.3465981353510372, -1.5975864015013592, 0.5689202269970073, 0.2332020626930963, -0.0292296965735871, 0.0189059943241274, 0.0350774923109639, -0.0269423512885117, -0.0051823922009606, 0.0001108055655560, 0.1123949306334719, 0.2710793844980740, -0.1207212028238607, 0.7044546015024772, -0.7375264248310736, -0.1701670059220587, 0.1235767790783333, 0.0778793408452593, -0.0525111357556987, -0.5299079884146832, 0.0144438713570501, 0.3762876245266094, 60.9926355678292250},
{0.0209055286858383, -0.0868817619009388, 0.0005173380072870, 0.0129077969776720, -0.0081029978200420, 0.0154932131794553, 0.0005195995680576, -0.0031682395403367, -0.0101019379478319, 0.0080572591666284, -0.0296837087113419, 0.0300481348440281, 0.0279138471726613, -0.0383012859882784, 0.0210759177404538, -0.0094614974739923, 0.0006405933440138, -0.0014644442462355, 0.0009227201787371, 0.0004252567955581, -0.0000543650409797, 0.0001348589475788, -0.0077670585721932, -0.0151184362372370, 0.0092897557422575, 0.0089225004557603, 0.0006412545400692, 0.0010007305904771, 0.0024443197286724, 0.0071528550582825, -0.0340834065946872, 0.0004902249185976, 0.0268811797248623, -0.0043139978701637, -2.7681778262618923},
{0.2877023492805959, -0.6274231097346499, -0.0001366341588071, 0.1237220915818023, -0.1106015239339567, 0.1797288379322828, -0.1492088834650681, 0.2704448577866607, -0.3011388585887220, 0.2014940576091596, -0.2287016163093219, 0.3387134153834627, -0.1029214188998699, 0.0284441541143877, 0.1739602820845901, -0.1193834959698061, 0.0071580328949397, -0.0136180513645235, -0.0003298164534355, 0.0078041798030866, 0.0043179786596137, -0.0004610958478488, -0.0835886918572180, -0.2160162939045996, 0.1075415379730247, 0.0192079548593000, 0.1157045877087446, 0.0081716072300736, 0.0386608728410208, 0.0020211639879656, -0.2795120755846990, -0.0203715233414625, 0.3347718831648642, -0.0863463884573045, -35.7186892444330720},
{-0.2877028682280468, 0.6274329443515009, 0.0001364777837009, 0.8762780710522423, 0.1105993867610872, -0.1797267010923533, 0.1492115246306319, -0.2704444356984515, 0.3011323849686137, -0.2014904850335332, 0.2287063916834761, -0.3387193646778411, 0.1029192406342169, -0.0284412101567766, -0.1739624190054833, 0.1193855174048554, -0.0071580275205025, 0.0136181403646212, 0.0003296955727793, -0.0078040060595256, -0.0043180844557266, 0.0004610809165453, 0.0835899597077029, 0.2160144656241496, -0.1075435947927539, -0.0192107478288682, -0.1156988728299843, -0.0081732305236926, -0.0386639828531186, -0.0020171884612359, 0.2795189385808891, 0.0203558102674037, -0.3347609056200158, 0.0863449162150724, 35.7187592798822000},
{0.0085695044077545, -0.0158857991437130, -0.0001428051148566, 0.0146977446035655, -0.0045620764444792, 0.0060978402445201, -0.0041841148038617, 0.0289799332655609, -0.0141764152414577, 0.0115780088582627, 0.0036896564610389, 0.0014540298458229, -0.0293978539061074, 0.0159503332195102, 0.0106505526508752, -0.0045157748166742, -0.0003634242970481, 0.0002163972105271, 0.0002515319027396, 0.0001115838432552, 0.0002019997517347, -0.0000706222066041, -0.0032462833910488, -0.0038392537080457, -0.0011254500076456, -0.0211433135892016, 0.0243845153467451, -0.0017045462594373, 0.0026441370480456, 0.0017598493668153, -0.0130536109512752, 0.0053582779209969, 0.0054821861058322, -0.0042091100118181, -1.8497086529328151},
{0.0085053322654228, -0.0158581892889406, -0.0001455841642812, 0.0146985423812350, -0.0044935268647189, 0.0061334123586465, -0.0179091645963339, 0.0182026889221204, -0.0142446447744446, 0.0115436447566373, 0.0036026244933409, 0.0014539601972303, -0.0156762972329580, 0.0268834274002517, 0.0106800020153402, -0.0045038154010032, -0.0003604473231333, 0.0002162835255602, 0.0002472996537470, 0.0001124689698707, 0.0002016508268475, -0.0000699109137074, -0.0032491626636005, -0.0038546437537988, -0.0010897615044404, -0.0210832264579836, 0.0242754019792653, -0.0016665175914160, 0.0026313236687954, 0.0017683701795275, -0.0130982439905408, 0.0054168533183247, 0.0054498229955212, -0.0041974367532881, -1.8437745451986078},
{0.1020993225754406, 0.0174821721249460, 0.0000881782206166, 0.1028325325716944, -0.0326435742717946, 0.0107542170076597, -0.0635870641913412, 0.1477457525572321, -0.0681197622804012, 0.0547687881889246, 0.0455861078219985, -0.0058987834868361, -0.1639798061724584, 0.1370477806837116, 0.0684157193842716, -0.0273622311090426, -0.0019272804335834, 0.0012707211438167, 0.0009961236214700, 0.0013708672256966, 0.0002232149532389, -0.0002709841647313, -0.0190298951269697, 0.0048310713582986, -0.0556913451579334, -0.0713363899115643, 0.1206492716684936, -0.0152916426941938, 0.0058015524854325, 0.0046127613430036, -0.0833672143815655, 0.0249002285109749, 0.0545115815236976, -0.0255241252788086, -11.6699356389359430},
{0.1000628265816151, -0.0906145884340758, -0.0008952687007669, 0.0119326729063836, -0.0153370240316453, 0.0298811296443405, -0.0681014209883676, 0.0647789460304946, -0.0570781782720078, 0.0460033288720369, 0.0059131105743420, 0.0096999923785802, -0.1035084886719191, 0.0574160428668325, 0.0381279168521986, -0.0169902315011756, -0.0012757022407018, 0.0007866336463731, -0.0001011864046378, -0.0004404439087883, 0.0010643285143344, -0.0003015222121260, -0.0104298494354715, -0.0232731890803916, 0.0313916511044527, -0.0758227162792808, 0.0982358488147487, -0.0044102057099121, 0.0126148436477085, 0.0065395939940977, -0.0401141292182861, 0.0300537125305766, 0.0132917348547654, -0.0167454148476661, -13.4817000102182420},
{0.1008957658255519, -0.0907079627083064, -0.0008486856938872, 0.0118933251105660, -0.0151312950499829, 0.0296777133363457, -0.0682986876949290, 0.0648219174938191, -0.0572086787133129, 0.0462140083495923, 0.0063047028814364, 0.0096284538889364, -0.1034859563243599, 0.0562327653620712, 0.0393717020122007, -0.0173140604120102, -0.0013377678808723, 0.0008079287829178, -0.0000307264787592, -0.0004611366588787, 0.0010599168074736, -0.0003033060303024, -0.0110526578757319, -0.0226390802123942, 0.0146600336846787, -0.0868517877592401, 0.0961087172596800, -0.0043778601520993, 0.0127045270483845, 0.0070426410757136, -0.0250427427207394, 0.0425461322139315, 0.0141714798083898, -0.0165874264915835, -13.5612146976641870},
{0.3350974347218753, -0.2438322784226767, -0.0008703929169201, 0.2024379461961414, -0.0713270787180561, 0.0852453768204552, -0.2324944436668505, 0.3284204174813776, -0.2092774608646389, 0.1715365484149991, 0.0612372366578792, 0.0189259341371512, -0.4083363319279212, 0.2732392734663560, 0.1817628286680009, -0.0715879604965743, -0.0059270571815750, 0.0033003964598995, 0.0029838346400049, 0.0002616110644046, 0.0027991358423073, -0.0010183232747335, -0.0510886625752743, -0.0490610819512793, -0.0220613598121851, -0.2606519709929496, 0.3632187812084987, -0.0307158483057080, 0.0430485934344447, 0.0225197313236466, -0.2004893436528439, 0.1137439626700583, 0.1025833125275516, -0.0639063405830343, -42.2467473000367450},
{0.2667967628547704, -0.5405599876236881, -0.0006531639110258, 0.1108136510863955, -0.1025052590232987, 0.1642306440643241, -0.1497044007328751, 0.2735941257822668, -0.2910301044149905, 0.1934347517118601, -0.1989936301289184, 0.3086529204061650, -0.1308561370878042, 0.0667651004259280, 0.1528777892945318, -0.1099236594388128, 0.0065178609805280, -0.0121543196108689, -0.0012524719327544, 0.0073796633489612, 0.0043716247497965, -0.0005957956402956, -0.0758349266270674, -0.2008778361979257, 0.0982468859688251, 0.0102612299286885, 0.1150777144936759, 0.0071812280013511, 0.0362040486811429, -0.0051139955117659, -0.2454434759740527, -0.0208307041561334, 0.3078731812807676, -0.0820375940723838, -32.9505621698803140},
{0.0034524308767875, -0.0075288392670584, -0.0000016221537710, 0.0014846726479082, -0.0013272263331991, 0.0021567766238006, -0.0017905584776208, 0.0032451866386219, -0.0036134142819557, 0.0024178475889903, -0.0027444104085395, 0.0040645472077501, -0.0012349984217909, 0.0003414915369612, 0.0020873313050929, -0.0014326144710700, 0.0000858961427721, -0.0001634242167353, -0.0000039498907553, 0.0000936476649130, 0.0000518173927173, -0.0000055331633901, -0.0010029918969913, -0.0025921645321316, 0.0012902522389986, 0.0002305382428469, 0.0013884747761606, 0.0000981541420506, 0.0004638999722381, 0.0000242489239115, -0.0033540120637536, -0.0002442376124024, 0.0040169639840373, -0.0010361849812031, -0.4286250310170104},
{0.0243580587453936, -0.0944102551422693, 0.0005157287631612, 0.0143925856657602, -0.0094323562335121, 0.0176503314668896, -0.0012685623520449, 0.0000764151879302, -0.0137162736716447, 0.0104757253753584, -0.0324257654970982, 0.0341137166340489, 0.0266724238766629, -0.0379584380738866, 0.0231671155095020, -0.0108962526910135, 0.0007265105108446, -0.0016279458216392, 0.0009187767428095, 0.0005188746465058, -0.0000025084828601, 0.0001293369197444, -0.0087701292160915, -0.0177108042048010, 0.0105813703710802, 0.0091510529058514, 0.0020303498776284, 0.0010994118439310, 0.0029076278088944, 0.0071783164004957, -0.0374389468149889, 0.0002485827530385, 0.0308970074642033, -0.0053508522669485, -3.1968116787416787},
{0.2633452329859459, -0.5330236901409269, -0.0006532082475254, 0.1093294932935002, -0.1011705285222427, 0.1620761116372501, -0.1479389323868988, 0.2703531821710460, -0.2873973510873536, 0.1910098849983989, -0.1962746491062387, 0.3046039925928974, -0.1296038755363952, 0.0664425777192300, 0.1507466216587143, -0.1084748897721516, 0.0064315900068628, -0.0119904405060650, -0.0012481382357581, 0.0072843955480112, 0.0043210779329581, -0.0005904078499389, -0.0748305418313488, -0.1982898176932961, 0.0969616230275342, 0.0100447263610207, 0.1136703410381951, 0.0070851855899020, 0.0357498564410408, -0.0051539408229892, -0.2420896436939813, -0.0205490154154148, 0.3038010202453826, -0.0809783515782612, -32.5219717798948620},
{-0.0002421657581733, -0.0010675684506435, -0.0000193450337512, -0.0000036303770491, 0.0003401993792478, 0.0020216627766915, 0.0003025015406457, -0.0009054866958962, -0.0005451662385378, -0.0003956846988828, -0.0006450777080063, 0.0000740235578501, 0.0005511284297987, 0.0007205632143825, -0.0009977452507561, 0.0002094876615303, 0.0000091150572601, 0.0000049613967588, -0.0000096226588742, -0.0000235163071736, 0.0000203981661701, -0.0000007387710835, -0.0000651444203694, -0.0000701640055178, 0.0003524795634966, 0.0003145889950535, -0.0006910053491745, 0.0002310637621493, 0.0000638027891528, -0.0000041919261246, -0.0004920788233403, 0.0005916040678831, -0.0003402757138924, 0.0000921661653168, 0.3512353440986281},
{-0.0002516368074101, -0.0014823161894453, -0.0000260380391891, -0.0000143708009960, 0.0003557929610644, 0.0000909374421684, -0.0005092244468717, -0.0001056314678033, 0.0006341021608516, -0.0003914854065375, -0.0005501893042642, 0.0019420452044219, 0.0014265255342316, -0.0001708196280455, -0.0023142500150764, 0.0003397274965094, 0.0000107586419047, -0.0000070437296738, 0.0000006295946221, -0.0000250717046268, 0.0000214795198865, -0.0000014769196874, 0.0001399275260250, -0.0002614295101677, 0.0001586201846544, 0.0009992016725302, -0.0012742185515509, 0.0003005337968322, 0.0001933473787686, -0.0001174813520803, -0.0004749864751257, 0.0008168968613690, -0.0006871311658187, 0.0002057804696612, 0.3528616033832587},
{-0.0001997586030033, -0.0009893888645802, -0.0000073708620170, -0.0000463252132261, 0.0003015298802581, 0.0001894597064038, -0.0008493587054215, -0.0000984818331449, 0.0011596845403249, -0.0006558141518980, -0.0003008981048576, -0.0002608495207598, 0.0008055092545288, -0.0001342598591484, -0.0004092264950037, 0.0001881953698464, 0.0000118460160525, -0.0000050657385634, 0.0000435351138814, -0.0000186970621730, -0.0000175650150467, 0.0000046899442685, 0.0001246505517427, 0.0004370915605006, 0.0005292533134363, 0.0000128059199859, -0.0009079635314682, 0.0000615712489135, -0.0000366929967075, 0.0007288236492518, 0.0002314008449105, 0.0000400482254179, -0.0010016382775948, 0.0002530805267522, 0.3485394344628117},
{-0.0001327461095380, -0.0007200366033573, -0.0000076948661296, -0.0000003712194296, 0.0003371039638229, 0.0001709233360459, -0.0008973070052693, -0.0000528709262611, 0.0011602119936671, -0.0006516110032861, -0.0003869135800006, -0.0002835331160274, 0.0008939426677952, -0.0001066684477054, -0.0003187225442937, 0.0001000764909951, 0.0000225643513792, -0.0000104114915309, -0.0000076626882161, -0.0000048887737093, -0.0000007192194752, 0.0000042315818320, 0.0001467185688577, 0.0016104434752695, 0.0012664088286040, -0.0007291487546839, -0.0016542584554556, 0.0000458931657304, -0.0000844174848718, 0.0001123296651526, -0.0002797192773534, 0.0006913956129626, -0.0007489436473230, 0.0002728433937709, 0.3392731035908150},
{-0.0002194110325932, -0.0007071092936306, 0.0000056207856246, -0.0000041983741451, 0.0002712580719294, 0.0001886147210204, -0.0007562885637840, -0.0000931487083574, 0.0011816193678347, -0.0007116530818546, -0.0002749795334362, -0.0003525944827629, 0.0009915182026858, -0.0006596546552111, -0.0000048378523164, 0.0001973999757186, 0.0000190913156911, -0.0000128001983822, -0.0000050448247135, -0.0000020554310205, -0.0000029502223197, 0.0000050143675806, 0.0002637751786227, -0.0003622366428069, 0.0000084072861419, 0.0005386389864197, -0.0004892649552548, 0.0000546549775403, -0.0001159143869382, 0.0019590108921136, 0.0012609153013937, -0.0012460737799373, -0.0013714531105891, 0.0002028513531851, 0.3492681589166206},
{-0.0002550005403607, -0.0011240996264289, -0.0000203764230160, -0.0000038051771513, 0.0003581965797912, 0.0021288535917672, 0.0003184113883105, -0.0009534712020201, -0.0005739441300809, -0.0004166714493035, -0.0006791950994994, 0.0000780264138007, 0.0005802520613667, 0.0007587027964806, -0.0010507432540321, 0.0002206991333991, 0.0000095975590532, 0.0000052182969841, -0.0000101242564071, -0.0000247696264683, 0.0000214809974352, -0.0000007764083067, -0.0000686918918170, -0.0000738351783054, 0.0003712606820790, 0.0003305910838956, -0.0007270245504058, 0.0002432836286267, 0.0000670236490567, -0.0000041540492071, -0.0005181742129344, 0.0006228974116392, -0.0003583160312258, 0.0000970771432724, 0.3989906383243762},
{-0.0002649679090031, -0.0015608437224030, -0.0000274111615535, -0.0000151200284583, 0.0003748472710399, 0.0000957054838347, -0.0005364885155665, -0.0001111711828162, 0.0006682644129966, -0.0004126670344637, -0.0005796138260673, 0.0020450444998974, 0.0015024220087179, -0.0001802183906020, -0.0024366655738162, 0.0003576844706035, 0.0000113221426603, -0.0000074097915562, 0.0000006676869021, -0.0000264091894201, 0.0000226156816548, -0.0000015511583803, 0.0001473170089366, -0.0002751781293982, 0.0001669808294375, 0.0010522913211321, -0.0013419017898337, 0.0003164574066634, 0.0002036815549294, -0.0001238497707410, -0.0005002139832352, 0.0008604814876288, -0.0007238147352456, 0.0002167732948139, 0.4007029303655441},
{-0.0002103488052838, -0.0010420055855025, -0.0000077491455637, -0.0000487797586404, 0.0003173832080252, 0.0001995347344516, -0.0008943915341077, -0.0001035212374595, 0.0012208820745228, -0.0006904234596086, -0.0003165824416321, -0.0002748553964642, 0.0008483032673266, -0.0001414449802490, -0.0004309841037551, 0.0001981759770137, 0.0000124661313749, -0.0000053310764778, 0.0000458455256337, -0.0000196872350960, -0.0000184931687855, 0.0000049365396881, 0.0001314114901830, 0.0004600686745404, 0.0005572443837482, 0.0000133496539586, -0.0009558504392297, 0.0000648121922916, -0.0000385575662155, 0.0007673513994867, 0.0002439099657237, 0.0000418992480543, -0.0010546138584451, 0.0002664452749630, 0.3961517764288104},
{-0.0001397845922280, -0.0007584898691965, -0.0000080977627305, -0.0000003863175037, 0.0003551090618956, 0.0001797987005097, -0.0009446842563622, -0.0000557273267045, 0.0012216043091713, -0.0006860945018944, -0.0004076482161013, -0.0002982816204774, 0.0009409585419905, -0.0001119510632192, -0.0003357371332068, 0.0001053974496041, 0.0000237658824392, -0.0000109679133324, -0.0000080713717153, -0.0000051478319458, -0.0000007559213140, 0.0000044557716084, 0.0001544286155658, 0.0016958870212310, 0.0013335362119921, -0.0007676667087327, -0.0017421270415563, 0.0000483409944670, -0.0000888399791806, 0.0001182827309642, -0.0002946971035591, 0.0007282979649129, -0.0007889084766175, 0.0002874460285049, 0.3863946218445700},
{-0.0002310420597886, -0.0007444359323652, 0.0000059226864661, -0.0000044086823073, 0.0002857178392373, 0.0001985852123157, -0.0007961262070494, -0.0000979895828834, 0.0012436304010585, -0.0007491090008207, -0.0002892985751438, -0.0003716383935296, 0.0010438400516224, -0.0006943868219730, -0.0000048380248035, 0.0002076482306374, 0.0000200981718833, -0.0000134727014230, -0.0000053121163408, -0.0000021663678621, -0.0000031021937859, 0.0000052778550210, 0.0002779807174152, -0.0003815604115261, 0.0000084893932187, 0.0005675015423465, -0.0005151809628265, 0.0000574692617503, -0.0001220038512764, 0.0020627124834307, 0.0013279180070511, -0.0013119330446717, -0.0014443974135125, 0.0002135992846041, 0.3969189136047036},
{-0.0002068698085073, -0.0009812039719591, -0.0000090234930375, -0.0000270925355734, 0.0003480947883059, 0.0003054553073853, -0.0008338947071581, -0.0001597340373968, 0.0011412216751895, -0.0006900221225500, -0.0003619830584490, -0.0001673435393502, 0.0009882986268714, -0.0002611398644380, -0.0004460767695530, 0.0001848409064871, 0.0000165186262485, -0.0000080773913802, 0.0000204772598275, -0.0000139966074487, -0.0000081727970104, 0.0000043387783028, 0.0001687890089010, 0.0004332037462131, 0.0006019263926181, 0.0000749611011553, -0.0010786280504829, 0.0000866480475148, -0.0000432850714865, 0.0007973422613143, 0.0002032155060011, 0.0000543022136559, -0.0010332424356726, 0.0002520706800808, 0.3948879830227060},
{-0.0001964735237250, -0.0009320301016995, -0.0000085688396933, -0.0000257496420199, 0.0003306095387034, 0.0002902694905841, -0.0007922023734653, -0.0001516167017454, 0.0010837527965741, -0.0006552544703767, -0.0003437883195435, -0.0001589983178657, 0.0009386813481662, -0.0002475806403214, -0.0004241263117163, 0.0001756195632678, 0.0000156793353196, -0.0000076587963980, 0.0000194470648149, -0.0000132978229597, -0.0000077606226149, 0.0000041212252985, 0.0001601568517184, 0.0004114113841925, 0.0005719640763458, 0.0000712563503983, -0.0010246121994499, 0.0000822912234736, -0.0000410870056706, 0.0007572870586849, 0.0001925596513037, 0.0000520353898246, -0.0009815269902366, 0.0002395285067411, 0.3473399200703095},
{0.1360121833663172, -0.2685288910581372, -0.0002808826661085, 0.0545775963883253, -0.0505035744868737, 0.0739032420165170, -0.0754921861359168, 0.1389966987227857, -0.1473715554960745, 0.1007548773531634, -0.0932848218085336, 0.1488358773948634, -0.0691962032272668, 0.0313084107367206, 0.0845064288390523, -0.0584469511408049, 0.0029598379069127, -0.0059925073558453, -0.0009367084301914, 0.0036478438505576, 0.0025293526844062, -0.0003812632932765, -0.0406018266985581, -0.1148704069157337, 0.0338453217073377, 0.0327959653334487, 0.0556537521121409, 0.0038493480207739, 0.0194620631886390, -0.0205624191203855, -0.1423709580147176, 0.0025177999565868, 0.1694570973242833, -0.0412972091252338, -16.6696754265603530},
{0.0685352728724017, -0.1484079490777636, -0.0002496854598555, 0.0311555759257997, -0.0288300461584459, 0.0562331601000778, -0.0398100771418596, 0.0712587148915343, -0.0763280025102906, 0.0467103441137475, -0.0626924356369065, 0.0914430685573761, -0.0304871944004427, 0.0215983593134636, 0.0297374924327304, -0.0247871963548214, 0.0021933983279803, -0.0034078219328389, 0.0000918732975715, 0.0020605494248393, 0.0006985044513821, -0.0000445560428754, -0.0166794322137265, -0.0337645480706278, 0.0485012027661222, -0.0369517321683043, 0.0339819570484703, 0.0015590777714345, 0.0078893400677567, 0.0242873521388564, -0.0382014407312833, -0.0241111997327802, 0.0610983722867354, -0.0218580378758791, -8.6459789364055517},
{0.2045468053988638, -0.4169379168745311, -0.0005294908616561, 0.0857354821952129, -0.0793435372560934, 0.1301482339239586, -0.1153201951087155, 0.2102617293532187, -0.2236650377988980, 0.1474379934836709, -0.1559446665181042, 0.2402415283011101, -0.0996715233446869, 0.0529076416076211, 0.1142158571380483, -0.0832132016854663, 0.0051520568472659, -0.0093995912348666, -0.0008441600015805, 0.0057077877579343, 0.0032281094465797, -0.0004257680108525, -0.0572713846907211, -0.1486357314790762, 0.0823388570258604, -0.0041541983764237, 0.0896344307264201, 0.0054067599393856, 0.0273562604282623, 0.0037121377238655, -0.1805406841256275, -0.0216547347524491, 0.2305920257235046, -0.0631523325447563, -25.3156492701181720},
{0.0587976367217665, -0.1160935380095178, -0.0001207171275942, 0.0235938466442723, -0.0218252732825749, 0.0319405231784287, -0.0326420104362476, 0.0600974351015834, -0.0637259790265678, 0.0435701479213941, -0.0403163412096531, 0.0643283370997270, -0.0299006458811288, 0.0135467724669727, 0.0365011510866931, -0.0252561498359572, 0.0012792290464410, -0.0025905580803276, -0.0004042576315366, 0.0015770627446548, 0.0010930923952664, -0.0001648430401272, -0.0175522990062013, -0.0496563050573826, 0.0146238746198203, 0.0141582702799353, 0.0240889398090248, 0.0016566080662294, 0.0084028304362661, -0.0088785822364041, -0.0615289653410837, 0.0010391557239128, 0.0732859992142294, -0.0178490507745295, -7.2063322287667422},
{0.1773584059708282, -0.3501173531081961, -0.0003655960153455, 0.0711716156804867, -0.0658520505295420, 0.0963695986140211, -0.0984373600928096, 0.1812564954925421, -0.1921833090455550, 0.1313843687208473, -0.1216228944160911, 0.1940574963253438, -0.0902367388800285, 0.0408620888674320, 0.1101391286229614, -0.0761874219352592, 0.0038592388146624, -0.0078141621781767, -0.0012205539112051, 0.0047560181570604, 0.0032986251364826, -0.0004972498179025, -0.0529384992120376, -0.1497918097366651, 0.0440921062615971, 0.0428428263666693, 0.0725198561169725, 0.0050295146791545, 0.0253621125649976, -0.0267990841029565, -0.1856236310573545, 0.0032560248687320, 0.2209518457072537, -0.0538287415438512, -23.7331385017401320},
{0.1948101207498296, -0.3846288543104899, -0.0004028171440981, 0.0781744013290533, -0.0723303108727586, 0.1058263448333838, -0.1080976935285914, 0.1990859762770438, -0.2110970749966485, 0.1443130771270157, -0.1335993406863334, 0.2131778800850488, -0.0991168111260810, 0.0448193674920288, 0.1210565475206776, -0.0837103870648646, 0.0042391883632038, -0.0085830299142138, -0.0013415712298680, 0.0052252052171863, 0.0036224931045602, -0.0005460944446651, -0.0581330392193599, -0.1645367117360029, 0.0484337105031412, 0.0470039697673065, 0.0797117546480302, 0.0055136158288401, 0.0278767460102247, -0.0294686416629430, -0.2038641772147860, 0.0035446222251096, 0.2427349277768190, -0.0591479135635929, -23.8759724871031120},
{-0.0129112891989081, 0.0237432842319328, 0.0008822711189850, 0.0433822464360884, 0.0090150665190438, -0.0098565921338057, -0.0081655977327224, 0.0048344252556820, 0.0095409084159713, -0.0075815152725070, 0.0148561680045370, -0.0141275891743682, -0.0049998226904421, 0.0044438575548082, -0.0081009410173731, 0.0079700861348651, -0.0008020533270671, 0.0007160554325803, 0.0007723587942768, -0.0006948025169513, 0.0002959157688460, -0.0001270939711890, 0.0015589318162820, 0.0082903346265206, -0.0156902524273561, 0.0301109351263465, -0.0210246061599764, -0.0025117865381643, 0.0060442876128048, -0.0105305790261672, 0.0029807329625316, -0.0009250971779163, -0.0080372990345418, 0.0103041131703948, 4.9208610909890460},
{-0.0299075103315987, 0.1032543127875347, -0.0042962880873509, 0.0715550861782671, -0.0016335309687637, 0.0159577760670093, 0.0139820139821899, -0.0281398225951410, 0.0106982000776269, -0.0196288445675556, 0.0093082153958375, 0.0073638053219588, -0.0508847686176971, 0.1101477635532791, -0.0929788984564186, 0.0154181031998099, -0.0000531747992441, 0.0020781953029374, -0.0024834990122718, -0.0006831393053795, 0.0008255168495903, -0.0001162350484546, 0.0105137836418728, 0.0053703880121984, -0.0039468817461697, -0.0332072220925249, 0.0098227406983721, 0.0155778525236856, -0.0070130766163535, -0.0126785821550547, 0.0615293615697481, 0.0158980463821819, -0.0458012809617593, -0.0107050067804656, 3.8797460514530857},
{-0.0302161463134140, 0.1025120323001934, -0.0045171231122613, 0.0718121494474319, -0.0020974437341877, 0.0017354540627726, 0.0110677351450881, -0.0176574365809779, 0.0207882021385071, -0.0186489433193650, 0.0091244107067982, 0.0231146956939747, -0.0486420409622459, 0.1020929513012702, -0.1062256294075932, 0.0150959802008369, 0.0000610509168263, 0.0020067948185187, -0.0025573190316919, -0.0005558065459952, 0.0007401535348329, -0.0001158554897276, 0.0101796894802301, 0.0049603480355450, -0.0042062130470002, -0.0270777751295503, 0.0033686794057103, 0.0167915445941047, -0.0066349032127206, -0.0125081502126929, 0.0602091322958990, 0.0211055573336856, -0.0505431302460962, -0.0102207428948622, 3.9196586707424137},
{-0.1536717518580005, 0.2937040628776609, 0.0084486345819696, 0.4933557123067625, 0.0863192091522741, -0.1061424370041135, 0.0830182094470314, -0.1986087045471167, 0.2425951307130624, -0.1293529418853497, 0.0764911633047186, -0.2171225062530251, 0.2346077484858427, -0.1806482748113857, -0.0493982090597463, 0.0894453742418353, -0.0025234646094983, 0.0046830737022253, 0.0027532279152761, -0.0036922420285027, -0.0046318122952647, 0.0009829990333449, 0.0352110098118965, 0.1195838763220797, -0.0023993481219663, -0.0317604346740590, -0.0875615194399359, -0.0202586143849602, -0.0114361570523556, 0.0310271873560072, 0.0154134136612641, 0.0196973297598256, -0.1534310171498168, 0.0903422432591526, 17.0030789432898710},
{-0.0344318724408194, 0.0570377524243225, 0.0002222022746844, 0.1133238949163352, 0.0149767510970536, -0.0437563752294295, 0.0187329789675992, -0.0136360343941866, 0.0133081098582430, -0.0168699811722580, 0.0616959478963860, -0.0724460271792163, -0.0123602146391164, -0.0346286290769333, 0.0394575169938539, -0.0007411979648576, -0.0022006540636422, 0.0023266001028677, 0.0011662464425476, -0.0013715776055308, -0.0005636199995845, -0.0001500036338824, 0.0139679343184472, 0.0356346071946446, -0.0485135120310090, 0.0438677046658348, -0.0208696562108273, -0.0094843187322969, -0.0067123176776727, 0.0012771977702920, 0.0707790972535320, -0.0212904022967586, -0.0434126590361571, 0.0078285782943991, 4.8354191525857164},
{-0.0336153543441180, 0.0564649460983390, 0.0004333041327090, 0.1126009217237152, 0.0144998456659492, -0.0422932213585665, 0.0166299528886915, -0.0135334064563835, 0.0157278909142107, -0.0180378399876240, 0.0612970627731324, -0.0711684283225520, -0.0121932636996787, -0.0357357606967452, 0.0391128905357416, -0.0001898215223244, -0.0023376180256250, 0.0024189554901473, 0.0013448790578538, -0.0014613766138996, -0.0006262393292213, -0.0001176982449051, 0.0111762119339607, 0.0461895366497185, -0.0421926723525072, 0.0353034070276955, -0.0270949694979197, -0.0096578112993342, -0.0055235549641799, -0.0077246211512041, 0.0628954195075668, -0.0136338429833405, -0.0365882298762882, 0.0093629159600066, 4.7347745814811670},
{-14.0796853245643700, 69.4019255230158620, -3.6070180242801593, 31.7271445392212460, -7.6869907044511319, 5.4747923548480406, 9.1094592014208668, -2.8205068099938813, -4.3116758272062885, -3.6054377905756225, 11.8591548749304250, 8.1144002421363517, -54.8411918238099290, 85.1027975834302030, -55.4746197068032250, 4.1155573532473513, 0.0778887035616868, 1.1155187564178799, -1.7059701758982593, -0.1648884057219523, 0.5675330174135346, -0.0991080521173825, 9.1256846718944278, -2.9716034700020222, -5.5204394194821447, -33.7536619379343320, 24.9382393469805020, 9.4152537404476764, -6.7750378895650423, -6.6471185210462052, 45.0752119246696520, 10.3957437763453470, -26.9651221616204460, -13.5438226308421610, 1622.2189981010313000},
{-0.0993160563249475, 0.1962140414932689, 0.0000228698431063, 0.2866762927710418, 0.0445929329107276, -0.0276418606535010, 0.0481475201783389, -0.1036367605649398, 0.0994570405192168, -0.0776264452177497, 0.0601435077367376, -0.1016745991645432, 0.0437829393210334, -0.0052829709516894, -0.0726904545098152, 0.0450979688640810, -0.0025170104412163, 0.0053136907320897, -0.0005128151206175, -0.0029204244102289, -0.0008628010942735, 0.0000605131805013, 0.0173401451028682, 0.0773009281512210, -0.0260745181512737, 0.0233397601500839, -0.0803669659916131, 0.0055645343872066, -0.0057822767549183, -0.0081970295910162, 0.0760037796296587, 0.0269063054041665, -0.1206702586878141, 0.0334262872741782, 12.2048631471637630},
{-0.0997028388836171, 0.1921239668843187, -0.0002952932881346, 0.2870340487977742, 0.0442239591613061, -0.0568676369687123, 0.0307271805680265, -0.0908913423427314, 0.1192873502208530, -0.0760970556550235, 0.0600406542604576, -0.0704334220606650, 0.0586942071623027, -0.0118185593808827, -0.0989033674178798, 0.0447971256618683, -0.0023814434773459, 0.0051946513604494, -0.0005344168094498, -0.0028026434965851, -0.0009479979032554, 0.0000490954103686, 0.0166918351026508, 0.0768760513608312, -0.0278120505941712, 0.0346096182956283, -0.0910307745420160, 0.0073919664582217, -0.0049468427568359, -0.0080028736285944, 0.0750117886651978, 0.0351079759692914, -0.1297054378206523, 0.0348696176209089, 12.2514215538776940},
{-0.0971330104883917, 0.1693187161290686, 0.0002245464798247, 0.2851045925874876, 0.0430670319171135, -0.0619852638768461, 0.0328174319710223, -0.0843413515075534, 0.1163477859420646, -0.0781952854458912, 0.0691818350260373, -0.1019813463050231, 0.0342740783949809, -0.0177479248662000, -0.0538585682105914, 0.0414678195689506, -0.0030935036876959, 0.0050986341751671, 0.0012581739240615, -0.0030082902307578, -0.0015593824649759, 0.0001056592301746, 0.0273873251641478, 0.0791118962991911, -0.0325896422956704, 0.0319503268857390, -0.0863473491054474, -0.0000739879959037, -0.0048351553323141, 0.0034039415787604, 0.0964919160506560, -0.0080761601306938, -0.1140812512830721, 0.0353207482949265, 11.9739062494161780},
{-0.0959635420409937, 0.1716132317675453, 0.0003900402146592, 0.2858394668718261, 0.0386262065212367, -0.0589463280512299, 0.0325689946641101, -0.0848173620162374, 0.1187863275294737, -0.0789103336699886, 0.0683404982374213, -0.0979740216576092, 0.0312326790455793, -0.0208281451161318, -0.0489922863178279, 0.0404771686021626, -0.0030562338556282, 0.0052041664252662, 0.0003840107819009, -0.0027153971636130, -0.0015346061100460, 0.0002256413639556, 0.0184888462348213, 0.1017966469318093, -0.0132114900887778, 0.0057400419972997, -0.0885296071438888, 0.0011930039815270, -0.0030532728636158, -0.0052747731302810, 0.0880510577304587, -0.0073433998544412, -0.1052724725034063, 0.0380152743290702, 11.7945245118254290},
{-0.0981098666716690, 0.1719575221363148, -0.0000523222536763, 0.2878295717240110, 0.0406286860383621, -0.0637628935315298, 0.0378567281187370, -0.0856091554783734, 0.1142236149498255, -0.0767227018376597, 0.0691110494062093, -0.1016978359517111, 0.0338140137638481, -0.0232152932093081, -0.0446791885252111, 0.0385671642572696, -0.0027027365124437, 0.0049253401365832, -0.0000598837058922, -0.0024449073788915, -0.0013911078318435, 0.0001395437455996, 0.0267681918793690, 0.0680116852761819, -0.0403285678349008, 0.0310378225038127, -0.0708789785235426, 0.0013436420303020, -0.0060240038774311, 0.0238191120186614, 0.1181545139708982, -0.0267058977597329, -0.1270609553556393, 0.0339565201554421, 12.0630807384494960},
{-0.3959451448533451, 0.3257471869534508, -0.0065552998621791, 1.1408804736030291, 0.1770468091020209, -0.4008618915810027, 0.1131233783991773, -0.0952192233327482, 0.1224702492200099, -0.2341366193264020, 0.5092251559820270, -0.4985324115859630, -0.1566582221845437, -0.3796017652930650, 0.3662944315436972, -0.0312437607918693, -0.0222582573056026, 0.0241613470086520, 0.0099574625509523, -0.0140748411875814, -0.0011957816527795, -0.0023177979356344, 0.0783410048863266, 0.4030011455582037, -0.4458807330440721, 0.9614173331474094, -0.7872903799239844, -0.0177493557008504, -0.0298118908563695, -0.0419884754012603, 0.7445692622564178, -0.2217327693413148, -0.4240318438315186, 0.0949518127280391, 49.2711014024365990},
{-0.2264051223272404, 1.8354804612558830, -0.0039035147569356, 1.1947725948442149, -0.0767866687725648, -0.2294670732486455, 0.6214783642080208, -0.3597333905640825, 0.2694411126684982, -0.1721849072402096, 0.4885583580985707, -0.4718413323133844, 0.1046810293639857, 0.2956799283188271, -0.4813763664734070, 0.1585550956780970, 0.0030411437549874, 0.0047709011461193, -0.0007420874875307, -0.0027645749394496, -0.0111011073635662, 0.0021202438348517, 0.1489474457902798, 0.2296952825576429, -0.0923096187193385, -0.6670042129377932, 0.3293603656397397, -0.0389977500892415, -0.0289849712984161, -0.0884936574342077, 0.2036683767716609, 0.2405219505726411, -0.5347861711496913, 0.0958246211114297, 74.0160667364418710},
{-0.1875007943221468, 0.3127797599416018, 0.0205520606944885, 0.5782081921524800, 0.2010078882002435, -0.2147108670303940, -0.1635144583391353, 0.1260640543872493, 0.0738682240607037, -0.1010460570833593, 0.3117692016083796, -0.3211359134587431, -0.0383463164551780, -0.1190017838933659, 0.0190786038391545, 0.1098486841806821, -0.0164834783119397, 0.0114955881785218, 0.0193256866324975, -0.0147357671693827, 0.0066100484176758, -0.0031820230106827, 0.0130206521058014, 0.1343119969920072, -0.3673495332896703, 0.8461468740846128, -0.5236466381969593, -0.0673698426882684, 0.1081480248810983, -0.2339712382790710, -0.0102490239506697, 0.0440155919603304, -0.1049530867231787, 0.1991011088962695, 87.6097195775660400},
{-0.0003656964516760, 0.0121543512679916, 0.0000590269307387, 0.0070951162238793, -0.0017177156588199, -0.0018044431251692, 0.0068645964861099, -0.0044737894197933, 0.0034496125483293, -0.0013999287373998, 0.0025024897399725, -0.0030380629557544, 0.0039439972603133, 0.0003707926675615, -0.0031561084821340, 0.0008132400377778, 0.0000945083365938, -0.0000571575565859, -0.0000085144538677, 0.0000466019863069, -0.0001862284498993, 0.0000437195245639, 0.0019450299439321, 0.0018171553404484, 0.0006894016263453, -0.0122939344913339, 0.0074359787148506, -0.0008265186512782, -0.0003090521485646, -0.0001140825638907, 0.0028306991064194, -0.0005145752649027, -0.0032518853033341, 0.0001768683291724, 0.7968932405040532},
{-1.5970859272244204, 1.5085765516148351, 0.0678300223436542, 4.4312821321549123, 2.3118133952386453, -0.9243247175331238, -1.7426274520117564, -0.2440023263501361, 3.5201514412445518, -3.1068931777294888, -0.0360285216029853, -0.5795560419818118, -0.2978155481272516, 2.1056710041821676, -3.8429282110005976, 2.2441241245815937, -0.1690210475619166, 0.1715532690893075, 0.0847904392502789, -0.1808508904140471, 0.1236934430527356, -0.0383861312556659, 1.7046800767595651, 0.0396440479957814, -1.4197352366811988, 6.0608093596862638, -6.1917975456468577, -0.0243374501330512, 1.5726230209795404, -1.7627383600833546, 3.6220089114245466, -3.7611866657935429, 0.0506610161454383, 0.4795832495411527, 183.5321796184933900},
};

////////////////////////////////////////////////////////////////////////////////
///////////////////////////3.函数声明///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///////////////////////////4.函数定义///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////