#include "postgres.h"

#include <ctype.h>
#include <limits.h>

#include "utils/builtins.h"
#include "utils/varlena.h"
/* 
 * data from vdjfasta
 */

#define arraysize(a)  (sizeof(a)/sizeof(const char *))
#define MAXCOUNT 65535
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

const char *h1_labels[] = {"IGHV1-18","IGHV1-2","IGHV1-24","IGHV1-3","IGHV1-45","IGHV1-46","IGHV1-58",
	"IGHV1-69/IGHV1-69D","IGHV1-69-2","IGHV1-8","IGHV2-26","IGHV2-5","IGHV2-70","IGHV3-11","IGHV3-13",
	"IGHV3-15","IGHV3-20","IGHV3-21/IGHV3-48","IGHV3-23/IGHV3-23D","IGHV3-30/IGHV3-30-3/IGHV3-64",
	"IGHV3-30-5/IGHV3-33/IGHV3-NL1","IGHV3-35","IGHV3-43","IGHV3-49","IGHV3-53/IGHV3-66","IGHV3-7",
	"IGHV3-72","IGHV3-73","IGHV3-74","IGHV3-9","IGHV4-28","IGHV4-30-2","IGHV4-30-4","IGHV4-31","IGHV4-34",
	"IGHV4-38-2","IGHV4-39","IGHV4-4","IGHV4-59","IGHV4-61","IGHV5-10-1","IGHV5-51","IGHV6-1","IGHV7-4-1"};

const char *h1_sequences[] = {"YTFTSYGIS","YTFTGYYMH","YTLTELSMH","YTFTSYAMH","YTFTYRYLH","YTFTSYYMH",
	"FTFTSSAVQ","GTFSSYAIS","YTFTDYYMH","YTFTSYDIN","FSLSNARMGVS","FSLSTSGVGVG","FSLSTSGMCVS",
	"FTFSDYYMS","FTFSSYDMH","FTFSNAWMS","FTFDDYGMS","FTFSSYSMN","FTFSSYAMS","FTFSSYAMH","FTFSSYGMH",
	"FTFSNSDMN","FTFDDYTMH","FTFGDYAMS","FTVSSNYMS","FTFSSYWMS","FTFSDHYMD","FTFSGSAMH","FTFSSYWMH",
	"FTFDDYAMH","YSISSSNWWG","GSISSGGYSWS","GSISSGDYYWS","GSISSGGYYWS","GSFSGYYWS","YSISSGYYWG",
	"GSISSSSYYWG","GSISSSNWWS","GSISSYYWS","GSVSSGSYYWS","YSFTSYWIS","YSFTSYWIG","DSVSSNSAAWN","YTFTSYAMN"};

const int h1_len = arraysize(h1_labels);

const char *h2_labels[] = {"IGHV1-18","IGHV1-2","IGHV1-24","IGHV1-3","IGHV1-45","IGHV1-46","IGHV1-58",
	"IGHV1-69/IGHV1-69D","IGHV1-69-2","IGHV1-8","IGHV2-26","IGHV2-5","IGHV2-70","IGHV3-11","IGHV3-13",
	"IGHV3-15","IGHV3-20","IGHV3-21","IGHV3-23/IGHV3-23D","IGHV3-30/IGHV3-30-3/IGHV3-30-5","IGHV3-33",
	"IGHV3-35","IGHV3-43","IGHV3-48","IGHV3-49","IGHV3-53/IGHV3-66","IGHV3-64","IGHV3-7","IGHV3-72",
	"IGHV3-73","IGHV3-74","IGHV3-9","IGHV3-NL1","IGHV4-28/IGHV4-30-4/IGHV4-31","IGHV4-30-2","IGHV4-34",
	"IGHV4-38-2","IGHV4-39","IGHV4-4","IGHV4-59/IGHV4-61","IGHV5-10-1","IGHV5-51","IGHV6-1","IGHV7-4-1"};

const char *h2_sequences[] = {"GWISAYNGNTNYA","GRINPNSGGTNYA","GGFDPEDGETIYA","GWINAGNGNTKYS","GWITPFNGNTNYA",
	"GIINPSGGSTSYA","GWIVVGSGNTNYA","GGIIPIFGTANYA","GLVDPEDGETIYA","GWMNPNSGNTGYA","AHIFSNDEKSYS",
	"ALIYWNDDKRYS","ALIDWDDDKYYS","SYISSSGSTIYYA","SAIGTAGDTYYP","GRIKSKTDGGTTDYA","SGINWNGGSTGYA",
	"SSISSSSSYIYYA","SAISGSGGSTYYA","AVISYDGSNKYYA","AVIWYDGSNKYYA","SGVSWNGSRTHYA","SLISWDGGSTYYA",
	"SYISSSSSTIYYA","GFIRSKAYGGTTEYT","SVIYSGGSTYYA","SAISSNGGSTYYA","ANIKQDGSEKYYV","GRTRNKANSYTTEYA",
	"GRIRSKANSYATAYA","SRINSDGSSTSYA","SGISWNSGSIGYA","SVIYSGGSSTYYA","GYIYYSGSTYYN","GYIYHSGSTYYN",
	"GEINHSGSTNYN","GSIYHSGSTYYN","GSIYYSGSTYYN","GEIYHSGSTNYN","GYIYYSGSTNYN","GRIDPSDSYTNYS",
	"GIIYPGDSDTRYS","GRTYYRSKWYNDYA","GWINTNTGNPTYA"};


const int h2_len = arraysize(h2_labels);

const char *l1_labels[] = {"IGKV1-12/IGKV1D-12/IGKV1D-16","IGKV1-16/IGKV1-27","IGKV1-17/IGKV1-6",
	"IGKV1-33/IGKV1D-33","IGKV1-39/IGKV1D-39","IGKV1-5","IGKV1-8/IGKV1-9","IGKV1-NL1","IGKV1D-13",
	"IGKV1D-17","IGKV1D-43","IGKV1D-8","IGKV2-24","IGKV2-28/IGKV2D-28","IGKV2-30/IGKV2D-30",
	"IGKV2-40/IGKV2D-40","IGKV2D-26","IGKV2D-29","IGKV3-11","IGKV3-15/IGKV3D-15","IGKV3-20",
	"IGKV3D-11","IGKV3D-20","IGKV3D-7","IGKV4-1","IGKV5-2","IGKV6-21/IGKV6D-21","IGLV1-36",
	"IGLV1-40","IGLV1-44","IGLV1-47","IGLV1-51","IGLV10-54","IGLV2-11/IGLV2-14/IGLV2-8","IGLV2-18",
	"IGLV2-23","IGLV3-1","IGLV3-10","IGLV3-12","IGLV3-16","IGLV3-19","IGLV3-21","IGLV3-22","IGLV3-25",
	"IGLV3-27","IGLV3-9","IGLV4-60","IGLV4-69","IGLV5-37","IGLV5-39/IGLV5-45","IGLV5-52",
	"IGLV6-57","IGLV7-43","IGLV7-46","IGLV8-61"};

const char *l1_sequences[] = {"RASQGISSWLA","RASQGISNYLA","RASQGIRNDLG","QASQDISNYLN","RASQSISSYLN",
	"RASQSISSWLA","RASQGISSYLA","RASQGISNSLA","RASQGISSALA","RARQGISNYLA","WASQGISSYLA","RMSQGISSYLA",
	"RSSQSLVHSDGNTYLS","RSSQSLLHSNGYNYLD","RSSQSLVYSDGNTYLN","RSSQSLLDSDDGNTYLD","RSSQSLLHSDGYTYLY",
	"KSSQSLLHSDGKTYLY","RASQSVSSYLA","RASQSVSSNLA","RASQSVSSSYLA","RASQGVSSYLA","GASQSVSSSYLA",
	"RASQSVSSSYLS","KSSQSVLYSSNNKNYLA","KASQDIDDDMN","RASQSIGSSLH","SGSSSNIGNNAVN","TGSSSNIGAGYDVH",
	"SGSSSNIGSNTVN","SGSSSNIGSNYVY","SGSSSNIGNNYVS","TGNSNNVGNQGAA","TGTSSDVGGYNYVS","TGTSSDVGSYNRVS",
	"TGTSSDVGSYNLVS","SGDKLGDKYAC","SGDALPKKYAY","GGNNIGSKAVH","SGEALPKKYAY","QGDSLRSYYAS","GGNNIGSKSVH",
	"SGDVLGENYAD","SGDALPKQYAY","SGDVLAKKYAR","GGNNIGSKNVH","TLSSGHSSYIIA","TLSSGHSSYAIA","TLPSDINVGSYNIY",
	"TLRSGINVGTYRIY","MLSSGFSVGDFWIR","TRSSGSIASNYVQ","ASSTGAVTSGYYPN","GSSTGAVTSGHYPY","GLSSGSVSTSYYPS"};

const int l1_len = arraysize(l1_labels);

const char *l2_labels[] = {"IGKV1-12/IGKV1-16/IGKV1-17/IGKV1-39/IGKV1-6/IGKV1D-12/IGKV1D-16/IGKV1D-17/IGKV1D-39",
	"IGKV1-27/IGKV1-8/IGKV1-9/IGKV1D-8","IGKV1-33/IGKV1D-33","IGKV1-5/IGKV1D-13","IGKV1-NL1","IGKV1D-43",
	"IGKV2-24","IGKV2-28/IGKV2D-28","IGKV2-30","IGKV2-40/IGKV2D-40","IGKV2D-26/IGKV2D-29","IGKV2D-30",
	"IGKV3-11/IGKV3D-11","IGKV3-15/IGKV3D-15/IGKV3D-7","IGKV3-20","IGKV3D-20","IGKV4-1","IGKV5-2",
	"IGKV6-21/IGKV6D-21","IGLV1-36","IGLV1-40","IGLV1-44","IGLV1-47","IGLV1-51","IGLV10-54","IGLV2-11",
	"IGLV2-14/IGLV2-18","IGLV2-23","IGLV2-8","IGLV3-1","IGLV3-10","IGLV3-12","IGLV3-16/IGLV3-25/IGLV3-27",
	"IGLV3-19","IGLV3-21","IGLV3-22","IGLV3-9","IGLV4-60","IGLV4-69","IGLV5-37","IGLV5-39/IGLV5-45",
	"IGLV5-52","IGLV6-57","IGLV7-43","IGLV7-46","IGLV8-61"};

const char *l2_sequences[] = {"AASSLQS","AASTLQS","DASNLET","DASSLES","AASRLES","YASSLQS","KISNRFS",
	"LGSNRAS","KVSNRDS","TLSYRAS","EVSNRFS","KVSNWDS","DASNRAT","GASTRAT","GASSRAT","DASSRAT","WASTRES",
	"EATTLVP","YASQSFS","YDDLLPS","GNSNRPS","SNNQRPS","RNNQRPS","DNNKRPS","RNNNRPS","DVSKRPS","EVSNRPS",
	"EGSKRPS","EVSKRPS","QDSKRPS","EDSKRPS","SDSNRPS","KDSERPS","GKNNRPS","YDSDRPS","EDSERYP","RDSNRPS",
	"LEGSGSYNKGS","LNSDGSHSKGD","YYSDSDKGQGS","YKSDSDKQQGS","YHSDSNKGQGS","EDNQRPS","STSNKHS","DTSNKHS",
	"STNTRSS"};

const int l2_len = arraysize(l2_sequences);


const char *l1f_labels[] = {"IGKV1-12/IGKV1D-12","IGKV1-16/IGKV1-17/IGKV1-27/IGKV1-33/IGKV1-39/IGKV1-NL1/IGKV1D-16/IGKV1D-33/IGKV1D-39",
"IGKV1-5","IGKV1-6","IGKV1-8","IGKV1-9","IGKV1D-13","IGKV1D-17","IGKV1D-43","IGKV1D-8","IGKV2-24","IGKV2-28/IGKV2D-28",
"IGKV2-30/IGKV2D-30","IGKV2-40/IGKV2D-40","IGKV2D-26","IGKV2D-29","IGKV3-11/IGKV3D-11/IGKV3D-20","IGKV3-15/IGKV3D-15",
"IGKV3-20","IGKV3D-7","IGKV4-1","IGKV5-2","IGKV6-21/IGKV6D-21","IGLV1-36","IGLV1-40","IGLV1-44/IGLV1-47","IGLV1-51",
"IGLV10-54","IGLV2-11","IGLV2-14/IGLV2-23","IGLV2-18","IGLV2-8","IGLV3-1","IGLV3-10","IGLV3-12","IGLV3-16","IGLV3-19",
"IGLV3-21","IGLV3-22","IGLV3-25","IGLV3-27","IGLV3-9","IGLV4-60","IGLV4-69","IGLV5-37","IGLV5-39","IGLV5-45","IGLV5-52",
"IGLV6-57","IGLV7-43","IGLV7-46","IGLV8-61"};


const char *l1f_sequences[] = {"DIQMTQSPSSVSASVGDRVTITC","DIQMTQSPSSLSASVGDRVTITC","DIQMTQSPSTLSASVGDRVTITC",
"AIQMTQSPSSLSASVGDRVTITC","AIRMTQSPSSFSASTGDRVTITC","DIQLTQSPSFLSASVGDRVTITC","AIQLTQSPSSLSASVGDRVTITC",
"NIQMTQSPSAMSASVGDRVTITC","AIRMTQSPFSLSASVGDRVTITC","VIWMTQSPSLLSASTGDRVTISC","DIVMTQTPLSSPVTLGQPASISC",
"DIVMTQSPLSLPVTPGEPASISC","DVVMTQSPLSLPVTLGQPASISC","DIVMTQTPLSLPVTPGEPASISC","EIVMTQTPLSLSITPGEQASISC",
"DIVMTQTPLSLSVTPGQPASISC","EIVLTQSPATLSLSPGERATLSC","EIVMTQSPATLSVSPGERATLSC","EIVLTQSPGTLSLSPGERATLSC",
"EIVMTQSPATLSLSPGERATLSC","DIVMTQSPDSLAVSLGERATINC","ETTLTQSPAFMSATPGDKVNISC","EIVLTQSPDFQSVTPKEKVTITC",
"QSVLTQPPSVSEAPRQRVTISC","QSVLTQPPSVSGAPGQRVTISC","QSVLTQPPSASGTPGQRVTISC","QSVLTQPPSVSAAPGQKVTISC",
"QAGLTQPPSVSKGLRQTATLTC","QSALTQPRSVSGSPGQSVTISC","QSALTQPASVSGSPGQSITISC","QSALTQPPSVSGSPGQSVTISC",
"QSALTQPPSASGSPGQSVTISC","SYELTQPPSVSVSPGQTASITC","SYELTQPPSVSVSPGQTARITC","SYELTQPHSVSVATAQMARITC",
"SYELTQPPSVSVSLGQMARITC","SSELTQDPAVSVALGQTVRITC","SYVLTQPPSVSVAPGKTARITC","SYELTQLPSVSVSPGQTARITC",
"SYELMQPPSVSVSPGQTARITC","SYELTQPSSVSVSPGQTARITC","SYELTQPLSVSVALGQTARITC","QPVLTQSSSASASLGSSVKLTC",
"QLVLTQSPSASASLGASVKLTC","QPVLTQPPSSSASPGESARLTC","QPVLTQPTSLSASPGASARFTC","QAVLTQPASLSASPGASASLTC",
"QPVLTQPSSHSASSGASVRLTC","NFMLTQPHSVSESPGKTVTISC","QTVVTQEPSLTVSPGGTVTLTC","QAVVTQEPSLTVSPGGTVTLTC",
"QTVVTQEPSFSVSPGGTVTLTC"};

const int l1f_len = arraysize(l1f_sequences);

const char *l2f_labels[] = {"IGKV1-12/IGKV1-33/IGKV1-39/IGKV1-5/IGKV1-6/IGKV1-8/IGKV1-9/IGKV1D-12/IGKV1D-13/IGKV1D-33/IGKV1D-39","IGKV1-16",
	"IGKV1-17","IGKV1-27","IGKV1-NL1","IGKV1D-16","IGKV1D-17","IGKV1D-43","IGKV1D-8","IGKV2-24","IGKV2-28/IGKV2-40/IGKV2D-28/IGKV2D-40",
	"IGKV2-30/IGKV2D-30","IGKV2D-26","IGKV2D-29","IGKV3-11/IGKV3-15/IGKV3-20/IGKV3D-11/IGKV3D-15/IGKV3D-7","IGKV3D-20","IGKV4-1","IGKV5-2",
	"IGKV6-21/IGKV6D-21","IGLV1-36","IGLV1-40/IGLV1-44/IGLV1-47/IGLV1-51","IGLV10-54","IGLV2-11/IGLV2-14/IGLV2-23/IGLV2-8","IGLV2-18",
	"IGLV3-1","IGLV3-10","IGLV3-12","IGLV3-16","IGLV3-19/IGLV3-21/IGLV3-25/IGLV3-9","IGLV3-22","IGLV3-27","IGLV4-60","IGLV4-69",
	"IGLV5-37","IGLV5-39","IGLV5-45","IGLV5-52","IGLV6-57","IGLV7-43","IGLV7-46","IGLV8-61"};

const char *l2f_sequences[] = {"WYQQKPGKAPKLLIY","WFQQKPGKAPKSLIY","WYQQKPGKAPKRLIY","WYQQKPGKVPKLLIY",
	"WYQQKPGKAPKLLLY","WYQQKPEKAPKSLIY","WFQQKPGKVPKHLIY","WYQQKPAKAPKLFIY","WYQQKPGKAPELLIY","WLQQRPGQPPRLLIY",
	"WYLQKPGQSPQLLIY","WFQQRPGQSPRRLIY","WFLQKARPVSTLLIY","WYLQKPGQPPQLLIY","WYQQKPGQAPRLLIY","WYQQKPGLAPRLLIY","WYQQKPGQPPKLLIY",
	"WYQQKPGEAAIFIIQ","WYQQKPDQSPKLLIK","WYQQLPGKAPKLLIY","WYQQLPGTAPKLLIY","WLQQHQGHPPKLLSY","WYQQHPGKAPKLMIY","WYQQPPGTAPKLMIY",
	"WYQQKPGQSPVLVIY","WYQQKSGQAPVLVIY","WYQQKPGQDPVLVIY","WYQQKPGQFPVLVIY","WYQQKPGQAPVLVIY","WYQQKPGQAPELVIY","WFQQKPGQAPVLVIY",
	"WHQQQPGKAPRYLMK","WHQQQPEKGPRYLMK","WYQQKPGSPPRYLLY","WYQQKPGSLPRYLLR","WYQQKPGSPPQYLLR","WYQQKPGNPPRYLLY","WYQQRPGSSPTTVIY",
	"WFQQKPGQAPRALIY","WFQQKPGQAPRTLIY","WYQQTPGQAPRTLIY"};

const int l2f_len = arraysize(l2f_sequences);

const char *h1f_labels[] = {"IGHV1-18/IGHV1-2/IGHV1-3/IGHV1-46/IGHV1-8","IGHV1-24","IGHV1-45","IGHV1-58","IGHV1-69/IGHV1-69D",
"IGHV1-69-2","IGHV2-26","IGHV2-5","IGHV2-70","IGHV3-11","IGHV3-13/IGHV3-35/IGHV3-48/IGHV3-64/IGHV3-66/IGHV3-7/IGHV3-72/IGHV3-74",
"IGHV3-15/IGHV3-21","IGHV3-20","IGHV3-23/IGHV3-23D","IGHV3-30/IGHV3-30-3/IGHV3-30-5/IGHV3-33","IGHV3-43","IGHV3-49","IGHV3-53",
"IGHV3-73","IGHV3-9","IGHV3-NL1","IGHV4-28","IGHV4-30-2","IGHV4-30-4/IGHV4-31","IGHV4-34","IGHV4-38-2","IGHV4-39","IGHV4-4",
"IGHV4-59/IGHV4-61","IGHV5-10-1","IGHV5-51","IGHV6-1","IGHV7-4-1"};

const char *h1f_sequences[] = {"QVQLVQSGAEVKKPGASVKVSCKASG","QVQLVQSGAEVKKPGASVKVSCKVSG","QMQLVQSGAEVKKTGSSVKVSCKASG",
"QMQLVQSGPEVKKPGTSVKVSCKASG","QVQLVQSGAEVKKPGSSVKVSCKASG","EVQLVQSGAEVKKPGATVKISCKVSG","QVTLKESGPVLVKPTETLTLTCTVSG",
"QITLKESGPTLVKPTQTLTLTCTFSG","QVTLRESGPALVKPTQTLTLTCTFSG","QVQLVESGGGLVKPGGSLRLSCAASG","EVQLVESGGGLVQPGGSLRLSCAASG",
"EVQLVESGGGLVKPGGSLRLSCAASG","EVQLVESGGGVVRPGGSLRLSCAASG","EVQLLESGGGLVQPGGSLRLSCAASG","QVQLVESGGGVVQPGRSLRLSCAASG",
"EVQLVESGGVVVQPGGSLRLSCAASG","EVQLVESGGGLVQPGRSLRLSCTASG","EVQLVESGGGLIQPGGSLRLSCAASG","EVQLVESGGGLVQPGGSLKLSCAASG",
"EVQLVESGGGLVQPGRSLRLSCAASG","QVQLVESGGGVVQPGGSLRLSCAASG","QVQLQESGPGLVKPSDTLSLTCAVSG","QLQLQESGSGLVKPSQTLSLTCAVSG",
"QVQLQESGPGLVKPSQTLSLTCTVSG","QVQLQQWGAGLLKPSETLSLTCAVYG","QVQLQESGPGLVKPSETLSLTCAVSG","QLQLQESGPGLVKPSETLSLTCTVSG",
"QVQLQESGPGLVKPPGTLSLTCAVSG","QVQLQESGPGLVKPSETLSLTCTVSG","EVQLVQSGAEVKKPGESLRISCKGSG","EVQLVQSGAEVKKPGESLKISCKGSG",
"QVQLQQSGPGLVKPSQTLSLTCAISG","QVQLVQSGSELKKPGASVKVSCKASG"};

const int h1f_len = arraysize(h1f_sequences);

const char *h2f_labels[] = {"IGHV1-18/IGHV1-2/IGHV1-46/IGHV1-69/IGHV1-69D/IGHV7-4-1","IGHV1-24","IGHV1-3","IGHV1-45",
"IGHV1-58","IGHV1-69-2","IGHV1-8","IGHV2-26/IGHV2-5/IGHV2-70","IGHV3-11",
"IGHV3-13","IGHV3-15/IGHV3-20/IGHV3-21/IGHV3-23/IGHV3-23D/IGHV3-30/IGHV3-30-3/IGHV3-30-5/IGHV3-33/IGHV3-43/IGHV3-48/IGHV3-53/IGHV3-66/IGHV3-7/IGHV3-72/IGHV3-9/IGHV3-NL1",
"IGHV3-35","IGHV3-49","IGHV3-64","IGHV3-73","IGHV3-74","IGHV4-28/IGHV4-30-2/IGHV4-30-4/IGHV4-34/IGHV4-38-2/IGHV4-39/IGHV4-59/IGHV4-61",
"IGHV4-31","IGHV4-4","IGHV5-10-1/IGHV5-51","IGHV6-1"};

const char *h2f_sequences[] = {"WVRQAPGQGLEWM","WVRQAPGKGLEWM","WVRQAPGQRLEWM","WVRQAPGQALEWM","WVRQARGQRLEWI",
"WVQQAPGKGLEWM","WVRQATGQGLEWM","WIRQPPGKALEWL","WIRQAPGKGLEWV","WVRQATGKGLEWV","WVRQAPGKGLEWV","WVHQAPGKGLEWV",
"WFRQAPGKGLEWV","WVRQAPGKGLEYV","WVRQASGKGLEWV","WVRQAPGKGLVWV","WIRQPPGKGLEWI","WIRQHPGKGLEWI","WVRQPPGKGLEWI",
"WVRQMPGKGLEWM","WIRQSPSRGLEWL"};

const int h2f_len = arraysize(h2f_sequences);

/*
 * compute the hamming distance beteen two strings
 * char * str1
 * char * str2
 *
 * returns int distance between them
 * or -1 if the strings are different lengths
 */
static inline int hdist(const char *str1, const char *str2);
static inline int hdist(const char *str1, const char *str2)
{
    int count = 0;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if (len1 != len2 || len1 == 0 || len2 == 0) {
	    return -1; 
    }

    for(int i = 0; i < len1; i++ ) {
        if (str1[i] != str2[i])  count++;
    }
    return count;
}

/*
 * compute levenshtein distance between two strings
 * returns number of edits to turn one string into another
 *
 */
static inline int ldist(const char *s1, const char *s2);
static inline int ldist(const char *s1, const char *s2)
{
    unsigned int s1len, s2len, x, y, lastdiag, olddiag, result, *column;
    s1len = strlen(s1);
    s2len = strlen(s2);
    
    //unsigned int column[s1len + 1]; // old C
    column = (unsigned int *)malloc((s1len+1)*sizeof(unsigned int));

    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x - 1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y - 1] + 1, lastdiag + (s1[y-1] == s2[x - 1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    result = column[s1len];
    free(column);
    return result;
};


/*
 * labels
 * finds the germline sequence closes to the argument, and returns the label for that germline
 * sequence - sequence to check
 * sequences - list of germline sequences
 * labels - list of labels for the sequences
 * seq_len -length of the array of sequences
 *
 *
 */
const char * label( const char * sequence, const char* sequences[], const char* labels[], const int seq_len );
const char * label( const char * sequence, const char* sequences[], const char* labels[], const int seq_len )
{
   
    int count;
    int   minvalue = MAXCOUNT;
    const char *mintype  = NULL;

    for (int s = 0; s < seq_len; s++ ) {
    	    count = ldist(sequence, sequences[s]);

      	    if (count > -1 && count < minvalue) {
		    minvalue = count;
		    mintype = labels[s];
	    }
    }	
    return mintype;
}


/*
 *  mindist
 *  resturns the smallest distance to any germline sequence in the cdr
 *  sequence - sequence to test
 *  sequences - list of germine sequenes
 *  seq_len - length of the sequences array
 *
 *  returns distance, or  MAXCOUNT if no lenghts match
 *
 *
 */
int mindist( const char * sequence, const char * sequences[], const int seq_len) ;
int mindist( const char * sequence, const char * sequences[], const int seq_len) 
{
    int count = -1;
    int minvalue = MAXCOUNT;
    const char *str2;
    const char *str1 = sequence;
    
    for (int s = 0; s < seq_len; s++ ) {
	    str2 = sequences[s];
    	    count = ldist(str1, str2);
      	    if (count > -1 && count < minvalue) {
		    minvalue = count; 
	    }
    }
    return minvalue;
}


/*
 *
 * set of postgresql functions 
 *
 */


PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(hammingdistance);
Datum
hammingdistance(PG_FUNCTION_ARGS)
{
    const char *str1, *str2;
    int count;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    str2 = text_to_cstring(PG_GETARG_TEXT_PP(1));

    count = hdist(str1, str2);
    if (count == -1)    PG_RETURN_NULL();
    PG_RETURN_INT32(count);
}


PG_FUNCTION_INFO_V1(levenshteindistance);
Datum
levenshteindistance(PG_FUNCTION_ARGS)
{   
    const char *str1, *str2;
    int count; 
                    
    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    str2 = text_to_cstring(PG_GETARG_TEXT_PP(1));
    
    count = ldist(str1, str2);
    if (count == -1)    PG_RETURN_NULL();
    PG_RETURN_INT32(count);
}   


PG_FUNCTION_INFO_V1(h1_germdist);
Datum
h1_germdist(PG_FUNCTION_ARGS)
{
    char *str1;
    int minvalue;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    minvalue = mindist(str1, h1_sequences, h1_len);

    if (minvalue == MAXCOUNT) PG_RETURN_NULL();
    PG_RETURN_INT32(minvalue);
}


PG_FUNCTION_INFO_V1(h1_bestgerm);
Datum
h1_bestgerm(PG_FUNCTION_ARGS)
{
    const char *str1;
    const char *mintype;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    mintype = label(str1, h1_sequences, h1_labels, h1_len);

    if (mintype == NULL) PG_RETURN_NULL();
    PG_RETURN_TEXT_P(cstring_to_text(mintype));
}



PG_FUNCTION_INFO_V1(h2_germdist);
Datum
h2_germdist(PG_FUNCTION_ARGS)
{
    char *str1;
    int minvalue;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    minvalue = mindist(str1, h2_sequences, h2_len);

    if (minvalue == MAXCOUNT) PG_RETURN_NULL();
    PG_RETURN_INT32(minvalue);
}


PG_FUNCTION_INFO_V1(h2_bestgerm);
Datum
h2_bestgerm(PG_FUNCTION_ARGS)
{
    const char *str1;
    const char *mintype;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    mintype = label(str1, h2_sequences, h2_labels, h2_len);

    if (mintype == NULL) PG_RETURN_NULL();
    PG_RETURN_TEXT_P(cstring_to_text(mintype));
}


PG_FUNCTION_INFO_V1(l1_germdist);
Datum
l1_germdist(PG_FUNCTION_ARGS)
{
    const char *str1;
    int minvalue;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    minvalue = mindist(str1, l1_sequences, l1_len);
   
    if (minvalue == MAXCOUNT) PG_RETURN_NULL();
    PG_RETURN_INT32(minvalue);
}


PG_FUNCTION_INFO_V1(l1_bestgerm);
Datum
l1_bestgerm(PG_FUNCTION_ARGS)
{
    const char *str1;
    const char *mintype;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    mintype = label(str1, l1_sequences, l1_labels, l1_len);

    if (mintype == NULL) PG_RETURN_NULL();
    PG_RETURN_TEXT_P(cstring_to_text(mintype));
}



PG_FUNCTION_INFO_V1(l2_germdist);
Datum
l2_germdist(PG_FUNCTION_ARGS)
{
    char *str1;
    int minvalue;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    minvalue = mindist(str1, l2_sequences, l2_len);

    if (minvalue == MAXCOUNT) PG_RETURN_NULL();
    PG_RETURN_INT32(minvalue);
}


PG_FUNCTION_INFO_V1(l2_bestgerm);
Datum
l2_bestgerm(PG_FUNCTION_ARGS)
{
    char *str1;
    const char *mintype;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    mintype = label(str1, l2_sequences, l2_labels, l2_len);

    if (mintype == NULL) PG_RETURN_NULL();
    PG_RETURN_TEXT_P(cstring_to_text(mintype));
}



PG_FUNCTION_INFO_V1(l1f_germdist);
Datum
l1f_germdist(PG_FUNCTION_ARGS)
{
    const char *str1;
    int minvalue;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    minvalue = mindist(str1, l1f_sequences, l1f_len);

    if (minvalue == MAXCOUNT) PG_RETURN_NULL();
    PG_RETURN_INT32(minvalue);
}


PG_FUNCTION_INFO_V1(l1f_bestgerm);
Datum
l1f_bestgerm(PG_FUNCTION_ARGS)
{
    const char *str1;
    const char *mintype;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    mintype = label(str1, l1f_sequences, l1f_labels, l1f_len);

    if (mintype == NULL) PG_RETURN_NULL();
    PG_RETURN_TEXT_P(cstring_to_text(mintype));
}

PG_FUNCTION_INFO_V1(l2f_germdist);
Datum
l2f_germdist(PG_FUNCTION_ARGS)
{
    const char *str1;
    int minvalue = MAXCOUNT;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    minvalue = mindist(str1, l2f_sequences, l2f_len);

    if (minvalue == MAXCOUNT) PG_RETURN_NULL();
    PG_RETURN_INT32(minvalue);
}


PG_FUNCTION_INFO_V1(l2f_bestgerm);
Datum
l2f_bestgerm(PG_FUNCTION_ARGS)
{
    const char *str1;
    const char *mintype;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    mintype = label(str1, l2f_sequences, l2f_labels, l2f_len);

    if (mintype == NULL) PG_RETURN_NULL();
    PG_RETURN_TEXT_P(cstring_to_text(mintype));
}

PG_FUNCTION_INFO_V1(h1f_germdist);
Datum
h1f_germdist(PG_FUNCTION_ARGS)
{
    const char *str1;
    int minvalue;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    minvalue = mindist(str1, h1f_sequences, h1f_len);

    if (minvalue == MAXCOUNT) PG_RETURN_NULL();
    PG_RETURN_INT32(minvalue);
}


PG_FUNCTION_INFO_V1(h1f_bestgerm);
Datum
h1f_bestgerm(PG_FUNCTION_ARGS)
{
    const char *str1;
    const char *mintype;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    mintype = label(str1, h1f_sequences, h1f_labels, h1f_len);

    if (mintype == NULL) PG_RETURN_NULL();
    PG_RETURN_TEXT_P(cstring_to_text(mintype));
}


PG_FUNCTION_INFO_V1(h2f_germdist);
Datum
h2f_germdist(PG_FUNCTION_ARGS)
{
    char *str1;
    int minvalue;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    minvalue = mindist(str1, h2f_sequences, h2f_len);

    if (minvalue == MAXCOUNT) PG_RETURN_NULL();
    PG_RETURN_INT32(minvalue);
}


PG_FUNCTION_INFO_V1(h2f_bestgerm);
Datum
h2f_bestgerm(PG_FUNCTION_ARGS)
{
    const char *str1;
    const char *mintype;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    mintype = label(str1, h2f_sequences, h2f_labels, h2f_len);

    if (mintype == NULL) PG_RETURN_NULL();
    PG_RETURN_TEXT_P(cstring_to_text(mintype));
}
