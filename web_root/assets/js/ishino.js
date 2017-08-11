// Ready
$(document).ready(function () {

	// GoTo Top Button
    var topBtn = $('#page-top');
    topBtn.hide();
    $(window).scroll(function () {
        if ($(this).scrollTop() > 100) {
            topBtn.fadeIn();
        } else {
            topBtn.fadeOut();
        }
    });
    topBtn.click(function () {
        $('body,html').animate({
            scrollTop: 0
        }, 500);
        return false;
    });
});

// Load
window.onload = locationHashChanged;

// HachChange
window.onhashchange = locationHashChanged;

var news_ary = {
	'#NewsMusicOversea': [ 'Music > Oversea', 'BARKS, RO69', 'rss' ],
	'#NewsMusicItem': [ 'Music > Item', 'RandoM, Supernice!', 'rss' ],
	'#NewsItIt': [ 'Tech > 一般・Business', 'ITpro, gihyo.jp, TechCrunch, THE BRIDGE, CNET Japan', 'rss' ],
	'#NewsItProgram': [ 'Tech > プログラム', 'CodeZine', 'rss' ],
	'#NewsItInfra': [ 'Tech > インフラ', 'ITpro Cloud, クラウドWatch, Think IT', 'rss' ],
	'#NewsItPosting': [ 'Tech > はてぶ・Qiita', 'はてな, Qiita', 'rss' ],
	'#NewsItCompany': [ 'Tech > 企業ブログ', 'cookpad, はてな, mercari, TORETA, LINE', 'rss' ],
	'#NewsHealth': [ 'Other > HealthCare', 'HeatlTech, マイナビ, 日経, ITmedia', 'rss' ],
	'#NewsCar': [ 'Other > Car', 'Carview, オートックワン', 'rss' ],
	'#NewsGame': [ 'Other > Game', 'SocailGameInfo, GameBusiness.jp, 4Gamer.net', 'rss' ],
	'#NewsItYuru': [ 'Other > ゆるネタ', 'Gigazine, ASCII', 'rss' ],
	'#Profile': [ "Profile", '', '' ],
	'#Blog': [ 'Blog', '', '' ]
};

function locationHashChanged() {
	var hash_val = location.hash
	var val_flg = 0;
	var str_01 = '';
	var str_02 = '';

	if ( hash_val == '#' || hash_val == '' ) { hash_val = '#NewsMusicOversea'; }

	Object.keys(news_ary).forEach( function(key) {
		if ( key == hash_val ) {
			str_01 = news_ary[hash_val][0];
			if ( news_ary[hash_val][2] == 'rss' ) {
				str_02 = '<i class="fa fa-rss" aria-hidden="true"></i>&nbsp;&nbsp;' + news_ary[hash_val][1];
			} else {
				str_02 = news_ary[hash_val][1];
			}

			$("#newsheader_01").html(str_01);
			$("#newsheader_02").html(str_02);
		}
	}, news_ary );

	// Goto Top
	$('body,html').animate({ scrollTop: 0 }, 500);

	if ( hash_val == '#Blog' ) {
		//$('#sidebar a.toggle').click();
	}

}

