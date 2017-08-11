<?php
/*****************************************************************************
 *  name        : News.inc
 *  function    : ニュースクラス
 *
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// ニュースクラス
//////////////////////////////////////////////////////////////////////////////
class ClsNews {

	/******************/
	/* コンストラクタ */
	/******************/
	function ClsNews() {
	}

	/******************************/
	/* 日付・ジャンルマスター取得 */
	/* 1 : その他                 */
	/* 2 : Music                  */
	/* 3 : PC                     */
	/******************************/
	function getMasterData() {

		$result = array();

		// yyyy
		$year = date( "Y" );
		$result['yyyy'][$year - 1] = $year - 1;
		$result['yyyy'][$year] = $year;
		$result['yyyy'][$year + 1] = $year + 1;
		// mm
		for ( $ix = 1 ; $ix <= 12 ; $ix++ ) {
			$result['mm'][sprintf( '%02d', $ix )] = sprintf( '%02d', $ix );
		}
		// dd
		for ( $ix = 1 ; $ix <= 31 ; $ix++ ) {
			$result['dd'][sprintf( '%02d', $ix )] = sprintf( '%02d', $ix );
		}

		// ジャンル
		$result['genre'][2] = '(02) Music';
		$result['genre'][3] = '(03) PC';
		$result['genre'][1] = '(01) その他';
		$result['genre'][90] = '(90) サイト更新';
		$result['genre'][12] = '(12) Music (Affiliate)';
		$result['genre'][13] = '(13) PC (Affiliate)';
		$result['genre'][11] = '(11) その他 (Affiliate)';

        return $result;
	}

	/****************/
	/* ニュース取得 */
	/****************/
	function getNews( $get_file = 'all', $file_max = 18, $year = 0 ) {

		// 取得ファイル種別
		//$get_num = -1;
		//if ( $get_file == 'music' ) { $get_num = 2; }
		//if ( $get_file == 'pc' ) { $get_num = 3; }

		// ファイル名取得
		$file_ary = array();
		if ( $year >= '2006' ) {
			$txt_path = NEWS_PATH_TXT.$year.'/';
		} else {
			$txt_path = NEWS_PATH_TXT;
		}
		$file_ary = DirFileGet( $txt_path );
		rsort( $file_ary ); // ソート

		// 結合
		$all_file = array();
		$regex = '/^[0-9]{8}_[0-9]{2}_[0-9]{2}.txt$/';
		foreach ( $file_ary as $key => $val ) {
			if ( preg_match( $regex, $val ) ) {
				$all_file[] = array( 'dir' => $txt_path, 'file_name' => $val );
			}
		}

		$result = array();
		for ( $ix = 0 ; $ix < count( $all_file ) ; $ix++ ) {

			// 各ファイル内容の取得
			$file_type = ( int )substr( $all_file[$ix]['file_name'], 12, 2 );
			$file_date = '['.substr( $all_file[$ix]['file_name'], 0, 4 ).'/'.substr( $all_file[$ix]['file_name'], 4, 2 ).'/'.substr( $all_file[$ix]['file_name'], 6, 2 ).']'; // 日付
			$file_name = $all_file[$ix]['dir'].$all_file[$ix]['file_name']; // ファイル名
			$file_data_tmp = array();
			$file_data_tmp = file( $file_name );

			$title = $file_data_tmp[0];
			$exp_data = explode( ' ', $file_data_tmp[1], 3 );
			$link = $exp_data[0];
			$link_from = $exp_data[1];
			$reg_img = '/^[0-9]{8}_[0-9]{2}_[0-9]{2}.*$/';
			if ( $year >= '2006' && preg_match( $reg_img, $exp_data[2] ) ) {
				$image_name = $year.'/'.$exp_data[2];
			} else {
				$image_name = $exp_data[2];
			}
			array_shift( $file_data_tmp );
			array_shift( $file_data_tmp );

			if ( $get_file == 'all' ) {
				$result[] = array( 'title' => str_replace( "\n", "", $title ),
									'link' => str_replace( "\n", "", $link ),
									'link_from' => str_replace( "\n", "", $link_from ),
									'image_name' => str_replace( "\n", "", $image_name ),
									'data' => str_replace( "\n", "", $file_data_tmp ),
									'update' => $file_date,
									'file_type' => $file_type,
									'file_name' => $file_name
								);
			} else if ( $get_file == 'music' ) {
				if ( $file_type == 2 || $file_type == 12 ) {
					$result[] = array( 'title' => str_replace( "\n", "", $title ),
										'link' => str_replace( "\n", "", $link ),
										'link_from' => str_replace( "\n", "", $link_from ),
										'image_name' => str_replace( "\n", "", $image_name ),
										'data' => str_replace( "\n", "", $file_data_tmp ),
										'update' => $file_date,
										'file_type' => $file_type,
										'file_name' => $file_name
									);
				}
			} else if ( $get_file == 'pc' ) {
				if ( $file_type == 3 || $file_type == 13 ) {
					$result[] = array( 'title' => str_replace( "\n", "", $title ),
										'link' => str_replace( "\n", "", $link ),
										'link_from' => str_replace( "\n", "", $link_from ),
										'image_name' => str_replace( "\n", "", $image_name ),
										'data' => str_replace( "\n", "", $file_data_tmp ),
										'update' => $file_date,
										'file_type' => $file_type,
										'file_name' => $file_name
									);
				}
			}

			// 取得トピック数
			if ( count( $result ) >= $file_max ) {
				break;
			}
		}
		//debug_print( $result, "result" );

		return $result;
	}

	/**********************/
	/* パラメータチェック */
	/**********************/
	function ParamCheck( $param ) {

		if ( !checkdate( $param['mm'], $param['dd'], $param['yyyy'] ) ) {
			return '日付エラー';
		}
		if ( $param['genre'] != 1 && $param['genre'] != 2 && $param['genre'] != 3 && $param['genre'] != 11 && $param['genre'] != 12 && $param['genre'] != 13 && $param['genre'] != 90 ) {
			return 'ジャンルエラー';
		}
		if ( $param['title'] == '' || $param['data'] == '' ) {
			return 'タイトル／本文エラー';
		}

		// image
		if ( $param['image'] != '' ) {
			if ( $param['origin'] == '' ) {
				return '情報元エラー';
			}
		}
		if ( $param['file']['name'] != '' ) {
			if ( $param['file']['size'] == 0 || $param['file']['error'] != 0 ) {
				if ( $param['file']['error'] == 1 ) {
					return( 'ファイルサイズエラー' );
				} else {
					return( 'ファイルエラー('.$param['file']['error'].')' );
				}
			}
			if ( !preg_match( "/\.jpg$|\.jpeg$|\.gif$|\.png$/", $param['file']['name'] ) ) {
				return( '画像形式エラー(jpg/jpeg/gif/png)' );
			}
		}

		return 1;
	}

	/****************/
	/* ニュース登録 */
	/****************/
	function makeNewsFile( $param ) {

		// 現状のファイル名取得
		$file_ary = array();
		$file_ary = DirFileGet( NEWS_PATH_TXT );
		rsort( $file_ary ); // ソート

		// ファイルチェック
		$chk_file_name = $param['yyyy'].sprintf( '%02d', $param['mm'] ).sprintf( '%02d', $param['dd'] );
		//debug_print( $chk_file_name, "chk_file_name" );

		// 同じ日付のファイルが存在するかチェック
		$exist_flag = 1;
		foreach ( $file_ary as $key => $val ) {
			if ( strstr( $val, $chk_file_name ) != false ) {
				$exist_flag = $val;
				break;
			}
		}

		// ファイル名作成
		$new_file_name = '';
		if ( $exist_flag == 1 ) {
			$new_file_header = $chk_file_name.'_01_'.sprintf( '%02d', $param['genre'] );
		} else {
			// 現在の連番取得
			$new_cnt = ( int )substr( $exist_flag, 9, 2 ) + 1;
			$new_file_header = $chk_file_name.'_'.sprintf( '%02d', $new_cnt ).'_'.sprintf( '%02d', $param['genre'] );
		}
		$new_file_name = $new_file_header.'.txt';
		//debug_print( $new_file_name, "new_file_name" );

		// 画像作成
		if ( $param['file']['name'] != '' ) {
			$ext = explode( '.', $param['file']['name'] );
			$put_file_name = $new_file_header.".".$ext[1];
			move_uploaded_file( $param['file']['tmp_name'], NEWS_PATH_FILE.$put_file_name );
		}

		// ファイル作成
		if ( !file_exists( $new_file_name ) ) {
			$this->WriteFile( NEWS_PATH_TXT.$new_file_name, $param['title'], $param['link'], $param['origin'], $put_file_name, $param['data'] );
		}

		return 1;
	}

	/****************/
	/* ニュース変更 */
	/****************/

	/****************/
	/* ニュース削除 */
	/****************/
	function DeleteNewsFile( $param ) {

		if ( file_exists( $param['file_name'] ) ) {
			$result = unlink( $param['file_name'] );
		}
		if ( $param['image_name'] != '' ) {
			if ( file_exists( NEWS_PATH_FILE.$param['image_name'] ) ) {
				$result = unlink( NEWS_PATH_FILE.$param['image_name'] );
			}
		}

		return result;
	}

	/******************/
	/* ファイル書込み */
	/******************/
	function WriteFile( $file_name, $title, $link, $origin, $put_file_name, $data ) {

		$fp = fopen( $file_name, "w" );
		flock( $fp, LOCK_EX );
		fwrite( $fp, $title );
		fwrite( $fp, "\n" );
		fwrite( $fp, $link." ".$origin." ".$put_file_name );
		fwrite( $fp, "\n" );
		fwrite( $fp, $data );
		fwrite( $fp, "\n" );
		fclose( $fp );

		return true;
	}
}

//////////////////////////////////////////////////////////////////////////////
// ライブ情報
//////////////////////////////////////////////////////////////////////////////
function GetLiveData( $file_max = 'all' ) {

	$result = array();
	$new_data = array();

	//$result[][] = array( year => "", date => "", artist => "", url => "", place => "" );
	$result[] = array( year => "2013", date => "10/19(土)", artist => "KISS", url => "http://www.udo.co.jp/Artists/Kiss/index.html#tokyo", place => "幕張メッセ" );
	$result[] = array( year => "2013", date => "10/10(木),11(金)", artist => "SUEDE", url => "http://okini.eplus.jp/2146/", place => "渋谷AX" );
	$result[] = array( year => "2013", date => "08/10(土),11(日)", artist => "SUMMER SONIC '13", url => "http://www.summersonic.com/2013/", place => "幕張メッセ" );
	$result[] = array( year => "2013", date => "08/08(木)", artist => "CHEAP TRICK", url => "http://www.creativeman.co.jp/news/#16622", place => "渋谷O-EAST" );
	$result[] = array( year => "2013", date => "08/01(木)", artist => "STEVE VAI", url => "http://eplus.jp/sys/T1U14P002100548P0030003P006001P0010175", place => "赤坂BLITZ" );
	$result[] = array( year => "2013", date => "07/26,27,28", artist => "FUJI ROCK FESTIVAL '13", url => "http://www.fujirockfestival.com/", place => "苗場スキー場" );
	$result[] = array( year => "2013", date => "07/17(水)", artist => "THE WINERY DOGS", url => "http://www.udo.co.jp/Artists/TheWineryDogs/index.html#tokyo", place => "日本青年館" );
	$result[] = array( year => "2013", date => "07/11(木),12(金)", artist => "SARAH BRIGHTMAN", url => "http://www.udo.co.jp/Artists/SarahBrightman/index.html", place => "東京国際フォーラム(A)" );
	$result[] = array( year => "2013", date => "06/21(金)", artist => "VAN HALEN", url => "http://www.udo.co.jp/Artists/VanHalen/index.html", place => "東京ドーム" );
	$result[] = array( year => "2013", date => "06/05-07(金)", artist => "HELLOWEEN / GAMMA RAY", url => "http://okini.eplus.jp/2561/index.html", place => "ZeppTokyo/横浜アリーナ" );
	$result[] = array( year => "2013", date => "05/11(土),12(日)", artist => "Ozzfest 2013", url => "http://eplus.jp/sys/T1U14P0010768P002088816P0050001", place => "幕張メッセ" );
	$result[] = array( year => "2013", date => "03/30(土),31(日)", artist => "PUNK SPRING '13", url => "http://www.punkspring.com/13/", place => "幕張メッセ" );
	$result[] = array( year => "2013", date => "03/12(火)", artist => "SANTANA", url => "http://www.udo.co.jp/Artists/Santana/index.html", place => "日本武道館" );
	$result[] = array( year => "2013", date => "03/11(月)", artist => "JOURNEY", url => "http://okini.eplus.jp/234/index.html", place => "日本武道館" );
	$result[] = array( year => "2013", date => "03/07(木),08(金)", artist => "DUFF McKAGAN", url => "http://okini.eplus.jp/32815/index.html", place => "duo MUSIC EXCHANGE" );
	$result[] = array( year => "2013", date => "02/19-21(木)", artist => "Adam Lambert", url => "http://okini.eplus.jp/39582", place => "渋谷公会堂" );
	$result[] = array( year => "2013", date => "02/15(金)", artist => "2CELLOS", url => "http://eplus.jp/sys/T1U14P0010413P002067365P0050001", place => "オーチャードホール" );
	$result[] = array( year => "2013", date => "02/06(木),07(金)", artist => "My Bloody Valentine", url => "http://okini.eplus.jp/24825/", place => "StudioCoast" );
	$result[] = array( year => "2013", date => "01/15(火),16(水)", artist => "Paul Gilbert", url => "http://www.udo.co.jp/Artists/PaulGilbert/index.html", place => "赤坂BLITZ" );
	$result[] = array( year => "2013", date => "01/11(金),12(土)", artist => "MUSE", url => "http://okini.eplus.jp/1368/", place => "埼玉スーパーアリーナ" );
	$result[] = array( year => "2012", date => "12/23(日),24(月)", artist => "PRINCESS PRINCESS", url => "http://ticket.pia.jp/pia/event.do?eventCd=1217983", place => "東京ドーム" );
	$result[] = array( year => "2012", date => "12/18(火)", artist => "Guns n' Roses", url => "http://okini.eplus.jp/194/", place => "ZeppTokyo" );
	$result[] = array( year => "2012", date => "12/03(月)", artist => "NICKELBACK", url => "http://www.barks.jp/news/?id=1000082447&ref=rss", place => "日本武道館" );
	$result[] = array( year => "2012", date => "11/09(金)", artist => "Norah Jones", url => "http://www.udo.co.jp/Artists/NorahJones/index.html", place => "日本武道館" );
	$result[] = array( year => "2012", date => "10/31(水)", artist => "SNUFF", url => "http://okini.eplus.jp/2134/", place => "渋谷O-EAST" );
	$result[] = array( year => "2012", date => "10/27(土)", artist => "LOUD PARK '12", url => "http://www.loudpark.com/12/", place => "埼玉スーパーアリーナ" );
	$result[] = array( year => "2012", date => "10/22,23,27(土)", artist => "Paul Weller", url => "http://t.pia.jp/feature/music/paulweller/index.html", place => "Zepp DiverCity" );
	$result[] = array( year => "2012", date => "10/15(月),16(火)", artist => "THE OFFSPRING", url => "http://tower.jp/article/news/2012/08/17/n06", place => "Zepp DiverCity" );
	$result[] = array( year => "2012", date => "10/15(月)", artist => "CANNIBAL CORPSE", url => "http://eplus.jp/sys/T1U14P0010413P0050001P006001P002078817", place => "渋谷CLUB QUATTRO" );
	$result[] = array( year => "2012", date => "09/24(月),25(火)", artist => "ASIA", url => "http://www.udo.co.jp/Artists/Asia/index.html", place => "渋谷公会堂" );
	$result[] = array( year => "2012", date => "09/22(土),23(日)", artist => "BUCK-TICK FEST 2012 ON PARADE", url => "http://ticket-news.pia.jp/pia/news.do?newsCd=201204250016", place => "千葉ポートパーク内特設野外ステージ" );
	$result[] = array( year => "2012", date => "09/18(火)", artist => "FEAR FACTORY", url => "http://okini.eplus.jp/44727/index.html", place => "渋谷CLUB QUATTRO" );
	$result[] = array( year => "2012", date => "08/29(水)", artist => "Ben E. King", url => "http://eplus.jp/sys/T1U14P0010413P0050001P006001P002045880", place => "中野サンプラザ" );
	$result[] = array( year => "2012", date => "08/23(木)", artist => "Napalm Death", url => "http://okini.eplus.jp/6607/", place => "渋谷CLUB QUATTRO" );
	$result[] = array( year => "2012", date => "08/18(土),19(日)", artist => "SUMMER SONIC '12", url => "http://www.summersonic.com/2012/", place => "幕張メッセ" );
	$result[] = array( year => "2012", date => "08/17(金)", artist => "EMINEM", url => "http://okini.eplus.jp/1364/", place => "QVCマリンフィールド" );
	$result[] = array( year => "2012", date => "08/16(木)", artist => "The Beach Boys", url => "http://okini.eplus.jp/55/", place => "QVCマリンフィールド" );
	$result[] = array( year => "2012", date => "07/27,28,29", artist => "FUJI ROCK FESTIVAL '12", url => "http://www.fujirockfestival.com/", place => "苗場スキー場" );
	$result[] = array( year => "2012", date => "07/24(火)", artist => "The Ventures", url => "http://okini.eplus.jp/964/", place => "中野サンプラザ" );
	$result[] = array( year => "2012", date => "07/17(火)", artist => "Feeder", url => "http://ticket-artist.pia.jp/pia/artists.do?artistsCd=11013568", place => "渋谷CLUB QUATTRO" );
	$result[] = array( year => "2012", date => "06/29(金)", artist => "Angel Witch", url => "http://okini.eplus.jp/36350/", place => "渋谷CLUB QUATTRO" );
	$result[] = array( year => "2012", date => "06/18(月),19(火)", artist => "布袋寅泰", url => "http://ticket.pia.jp/pia/event.do?eventCd=1213068", place => "日本武道館" );
	$result[] = array( year => "2012", date => "06/12(火),13(水)", artist => "アニメタルUSA/JAM Project ", url => "http://eplus.jp/sys/T1U14P0010413P0050001P006001P002071655", place => "ZeppTokyo" );
	$result[] = array( year => "2012", date => "06/10(日)", artist => "BUCK-TICK", url => "http://ticket-artist.pia.jp/pia/artists.do?artistsCd=11011678", place => "日比谷野音" );
	$result[] = array( year => "2012", date => "05/26(土),27(日)", artist => "Japan Blues ＆ Soul Carnival 2012", url => "http://ticket.pia.jp/pia/event.do?eventCd=1202708", place => "ZeppTokyo/日比谷野音" );
	$result[] = array( year => "2012", date => "05/24(木)", artist => "RAGE", url => "http://ticket.pia.jp/pia/event.do?eventCd=1206439", place => "渋谷O-EAST" );
	$result[] = array( year => "2012", date => "05/23(水)", artist => "NOEL GALLAGHER'S HIGH FLYING BIRDS", url => "http://eplus.jp/sys/T1U14P0050001P006001P0010768P002066378", place => "日本武道館" );
	$result[] = array( year => "2012", date => "05/23(水)", artist => "ANDREW W.K", url => "http://okini.eplus.jp/2230/", place => "LIQUIDROOM" );
	$result[] = array( year => "2012", date => "05/17(木),18(金)", artist => "MANIC STREET PREACHERS", url => "http://ticket.pia.jp/pia/event.do?eventCd=1209562", place => "StudioCoast" );
	$result[] = array( year => "2012", date => "05/16(水)", artist => "Royal Hunt", url => "http://eplus.jp/sys/T1U89P0101P006001P0050001P002004475P0030013P0006", place => "渋谷O-EAST" );
	$result[] = array( year => "2012", date => "05/10(木)", artist => "LADY GAGA", url => "http://yo-gaku.eplus2.jp/article/251349141.html", place => "埼玉スーパーアリーナ" );
	$result[] = array( year => "2012", date => "04/30(月)", artist => "DREAM THEATER", url => "http://ticket-artist.pia.jp/pia/artists.do?artistsCd=11024772", place => "横浜アリーナ" );
	$result[] = array( year => "2012", date => "04/21(土)", artist => "DOUBLE AXEMEN", url => "http://eplus.jp/sys/T1U89P0101P006001P0050001P002071192P0030001P0006", place => "渋谷BOXX" );
	$result[] = array( year => "2012", date => "04/18(水),19(木)", artist => "YES", url => "http://ticket-artist.pia.jp/pia/artists.do?artistsCd=11010119", place => "渋谷公会堂" );
	$result[] = array( year => "2012", date => "04/16(月),17(火)", artist => "EXTREME", url => "http://www.udo.co.jp/Artist/Extreme/index.html", place => "TOKYO DOME CITY HALL" );
	$result[] = array( year => "2012", date => "04/16(月)", artist => "AVENGED SEVENFOLD", url => "http://okini.eplus.jp/5486/index.html#announce", place => "ZeppTokyo" );
	$result[] = array( year => "2012", date => "04/13(金)", artist => "ARCH ENEMY", url => "http://okini.eplus.jp/2633/index.html#announce", place => "渋谷AX" );
	$result[] = array( year => "2012", date => "04/10(火),10(水)", artist => "Steve Lukather", url => "http://www.udo.co.jp/Artist/SteveLukather/", place => "赤坂BLITZ" );
	$result[] = array( year => "2012", date => "04/4(水)-7(土)", artist => "LENNY KRAVITZ", url => "http://eplus.jp/sys/T1U14P0010413P0050001P006001P002023091", place => "東京ドーム" );
	$result[] = array( year => "2012", date => "03/31(土),01(日)", artist => "PUNK SPRING '12", url => "http://www.punkspring.com/12/", place => "幕張メッセ" );
	$result[] = array( year => "2012", date => "03/29(木),30(金)", artist => "Michael Schenker", url => "http://okini.eplus.jp/6051/", place => "中野サンプラザ" );
	$result[] = array( year => "2012", date => "03/9(金)-11(日)", artist => "Cyndi Lauper", url => "http://ticket.pia.jp/pia/event.do?eventCd=1154750", place => "オーチャードホール" );
	$result[] = array( year => "2012", date => "03/9(金),10(土)", artist => "MARILYN MANSON", url => "http://eplus.jp/sys/T1U14P0010413P0050001P006001P002020588", place => "StudioCoast" );
	$result[] = array( year => "2012", date => "03/7(水)", artist => "Foo Fighters", url => "http://ticket-artist.pia.jp/pia/artists.do?artistsCd=11011883", place => "横浜アリーナ" );
	$result[] = array( year => "2012", date => "02/15(水),16(木)", artist => "Bryan Adams", url => "http://www.udo.co.jp/Artist/BryanAdams/index.html#tokyo", place => "日本武道館" );
	$result[] = array( year => "2012", date => "02/9(木),17(金)", artist => "Judas Priest", url => "http://ticket-artist.pia.jp/pia/artists.do?artistsCd=11025887", place => "日本武道館" );
	$result[] = array( year => "2012", date => "02/8(水),9(木)", artist => "EVANESCENCE", url => "http://okini.eplus.jp/2596/", place => "パシフィコ横浜/ZeppTokyo" );
	$result[] = array( year => "2012", date => "02/4(土),5(日)", artist => "Avril Lavigne", url => "http://ticket-artist.pia.jp/pia/artists.do?artistsCd=26050069", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2012", date => "01/24(火),25(水)", artist => "Zebrahead", url => "http://okini.eplus.jp/1355", place => "渋谷AX" );
	$result[] = array( year => "2012", date => "01/16(月),17(火)", artist => "2CELLOS", url => "http://www.udo.co.jp/Artist/2Cellos/index.html#tokyo", place => "赤坂BLITZ" );
	$result[] = array( year => "2011", date => "12/13(火),14(水)", artist => "ANTHRAX", url => "http://okini.eplus.jp/2690/", place => "渋谷O-EAST" );
	$result[] = array( year => "2011", date => "12/2,3,6,7", artist => "ERIC CLAPTON", url => "http://www.udo.co.jp/Artist/EricClaptonSteveWinwood/index.html#tokyo", place => "日本武道館" );
	$result[] = array( year => "2011", date => "11/28(月),30(水)", artist => "AEROSMITH", url => "http://www.udo.co.jp/Artist/Aerosmith/index.html", place => "東京ドーム" );
	$result[] = array( year => "2011", date => "11/07(月)", artist => "Def Leppard ", url => "http://okini.eplus.jp/131/", place => "東京国際フォーラム(A)" );
	$result[] = array( year => "2011", date => "10/24(月)", artist => "WHITESNAKE", url => "http://eplus.jp/sys/T1U14P0010413P0050001P006001P002060516", place => "赤坂BLITZ" );
	$result[] = array( year => "2011", date => "10/17(月)", artist => "LIMP BIZKIT", url => "http://okini.eplus.jp/1291/", place => "Studio Coast" );
	$result[] = array( year => "2011", date => "10/15(土)", artist => "LOUD PARK '11", url => "http://www.loudpark.com/11/", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2011", date => "10/03(火)-05(木)", artist => "Motley Crue", url => "http://www.barks.jp/news/?id=1000071943", place => "ZeppTokyo" );
	$result[] = array( year => "2011", date => "10/03(火),04(水)", artist => "BrianSetzer", url => "http://okini.eplus.jp/1448/", place => "渋谷C.C.Lemonホール" );
	$result[] = array( year => "2011", date => "09/11(日),13(火)", artist => "LINKIN PARK", url => "http://yo-gaku.eplus2.jp/article/194205536.html", place => "幕張メッセ/横浜アリーナ" );
	$result[] = array( year => "2011", date => "08/13(土),14(日)", artist => "SUMMER SONIC '11", url => "http://www.summersonic.com/2011/", place => "幕張メッセ" );
	$result[] = array( year => "2011", date => "07/29,30,31", artist => "FUJI ROCK FESTIVAL '11", url => "http://www.fujirockfestival.com/", place => "苗場スキー場" );
	$result[] = array( year => "2011", date => "06/14(火)", artist => "Children Of Bodom", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P2=053665:P5=0001:P6=001", place => "渋谷AX" );
	$result[] = array( year => "2011", date => "06/13(月),14(火)", artist => "NIGHT RANGER", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P2=008240:P5=0001:P6=001", place => "渋谷C.C.Lemonホール" );
	$result[] = array( year => "2011", date => "05/27(金),28(土)", artist => "SUM41", url => "http://okini.eplus.jp/2231/", place => "StudioCoast/ZeppTokyo" );
	$result[] = array( year => "2011", date => "05/23(月),24(火)", artist => "TOTO", url => "http://www.udo.co.jp/Artist/Toto/index.html", place => "パシフィコ横浜/日本武道館" );
	$result[] = array( year => "2011", date => "05/21(土),22(日)", artist => "Avril Lavigne", url => "http://okini.eplus.jp/1990/", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2011", date => "05/05(木),06(金)", artist => "ANTHRAX", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P2=023823:P5=0001:P6=001", place => "渋谷O-EAST" );
	$result[] = array( year => "2011", date => "04/22(金),25(月)", artist => "MR.BIG", url => "http://www.udo.co.jp/Artist/MrBig/index.html", place => "パシフィコ横浜/日本武道館" );
	$result[] = array( year => "2011", date => "04/20(水)", artist => "CATHEDRAL", url => "http://okini.eplus.jp/47223/", place => "渋谷CLUB QUATTRO" );
	$result[] = array( year => "2011", date => "04/13(水)-15(金)", artist => "JOHNNY WINTER", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P2=053361:P5=0001:P6=001", place => "Zepp Tokyo" );
	$result[] = array( year => "2011", date => "04/03(日)", artist => "PUNK SPRING '11", url => "http://www.punkspring.com/11/", place => "幕張メッセ" );
	$result[] = array( year => "2011", date => "03/16(水),17(木)", artist => "Slash", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P5=0001:P2=048979:P6=001", place => "Studio Coast" );
	$result[] = array( year => "2011", date => "03/15(火)", artist => "Cyndi Lauper", url => "http://ticket.pia.jp/pia/event.do?eventCd=1058020", place => "中京大学文化市民会館" );
	$result[] = array( year => "2011", date => "03/12(土),13(日)", artist => "IRON MAIDEN", url => "http://okini.eplus.jp/221/", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2011", date => "03/10(木),12(土)", artist => "MURDERDOLLS", url => "http://ticket-artist.pia.jp/pia/artists.do?artistsCd=27310022", place => "渋谷O-EAST/LIQUIDROOM" );
	$result[] = array( year => "2011", date => "03/05(土),06(日)", artist => "EAGLES", url => "http://www.udo.co.jp/Artist/Eagles/index.html", place => "東京ドーム" );
	$result[] = array( year => "2011", date => "03/01(火)", artist => "ELVIS COSTELLO", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P2=048964:P5=0001:P6=001", place => "オーチャードホール" );
	$result[] = array( year => "2011", date => "02/28(月)", artist => "Helloween/Stratovarius", url => "http://yo-gaku.eplus2.jp/article/167133861.html", place => "Zepp Tokyo" );
	$result[] = array( year => "2011", date => "02/05(日),06(月)", artist => "MY CHEMICAL ROMANCE", url => "http://okini.eplus.jp/2952/index.html", place => "横浜アリーナ" );
	$result[] = array( year => "2011", date => "01/17(月),18(火)", artist => "Sting", url => "http://www.udo.co.jp/Artist/Sting/index.html", place => "日本武道館" );
	$result[] = array( year => "2010", date => "12/06(月)", artist => "ORIANTHI", url => "http://okini.eplus.jp/40219/", place => "赤坂BLITZ" );
	$result[] = array( year => "2010", date => "11/30(火),01(水)", artist => "BON JOVI", url => "http://www.udo.co.jp/Artist/BonJovi/index.html", place => "東京ドーム" );
	$result[] = array( year => "2010", date => "11/26(金),27(土)", artist => "MANIC STREET PREACHERS", url => "http://eplus.jp/sys/T1U90P006001P0050001P002045204P0030002P0030001P0007P0010413P0491", place => "StudioCoast/横浜ベイホール" );
	$result[] = array( year => "2010", date => "11/17(水),18(木)", artist => "The Flaming Lips", url => "http://okini.eplus.jp/5468/", place => "Zepp東京" );
	$result[] = array( year => "2010", date => "10/25(月),26(火)", artist => "BAD COMPANY", url => "http://www.udo.co.jp/Artist/BadCompany/index.html", place => "東京国際フォーラム" );
	$result[] = array( year => "2010", date => "10/25(月)", artist => "THE JOHN BUTLER TRIO", url => "http://okini.eplus.jp/4024/index.html", place => "赤坂BLITZ" );
	$result[] = array( year => "2010", date => "10/16(土),17(日)", artist => "LOUD PARK '10", url => "http://www.loudpark.com/10/", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2010", date => "10/15(金)", artist => "HALFORD", url => "http://www.creativeman.co.jp/artist/2010/10halford/", place => "名古屋 CLUB DIAMOND HALL" );
	$result[] = array( year => "2010", date => "10/14(木)", artist => "KORN", url => "http://www.creativeman.co.jp/artist/2010/10korn/", place => "Studio Coast" );
	$result[] = array( year => "2010", date => "10/13(水)", artist => "Ozzy Osbourne", url => "http://www.creativeman.co.jp/artist/2010/10ozzy/", place => "東京国際フォーラム" );
	$result[] = array( year => "2010", date => "10/11(月),15(金)", artist => "Todd Rundgren", url => "http://okini.eplus.jp/464/index.html", place => "渋谷CLUB QUATTRO" );
	$result[] = array( year => "2010", date => "10/04(月),05(火)", artist => "KEITH EMERSON & GREG LAKE", url => "http://www.udo.co.jp/Artist/KeithEmersonGregLake/index.html", place => "渋谷C.C.Lemonホール" );
	$result[] = array( year => "2010", date => "09/30(木)", artist => "THE VOICES OF RAINBOW", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P5=0001:P2=043961:P6=001", place => "渋谷C.C.Lemonホール" );
	$result[] = array( year => "2010", date => "09/25(土),26(日)", artist => "METALLICA", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P2=042394:P5=0001:P6=001", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2010", date => "09/16(木)", artist => "FEEDER", url => "http://okini.eplus.jp/3683/index.html", place => "渋谷O－EAST" );
	$result[] = array( year => "2010", date => "08/27(金)", artist => "Foobastank", url => "http://okini.eplus.jp/2602/index.html", place => "赤坂BLITZ" );
	$result[] = array( year => "2010", date => "08/10(火),11(水)", artist => "THE SMASHING PUMPKINS", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P2=043981:P5=0001:P6=001", place => "Studio Coast" );
	$result[] = array( year => "2010", date => "08/07(土),08(日)", artist => "SUMMER SONIC '10", url => "http://www.summersonic.com/2010/", place => "幕張メッセ" );
	$result[] = array( year => "2010", date => "07/30,31,1", artist => "FUJI ROCK FESTIVAL '10", url => "http://www.fujirockfestival.com/", place => "苗場スキー場" );
	$result[] = array( year => "2010", date => "07/05(月),06(火)", artist => "MAXI PRIEST", url => "http://eplus.jp/sys/T1U90P006001P0050001P002041908P0030001P0481P0007P0010413P0491", place => "ブルーノート東京" );
	$result[] = array( year => "2010", date => "06/16(水)-19(土)", artist => "Larry Carlton & Tak Matsumoto", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P2=040942:P5=0001:P6=001", place => "Zepp大阪、福岡" );
	$result[] = array( year => "2010", date => "05/30(日)", artist => "MICHAEL BOLTON", url => "http://www.udo.co.jp/Artist/MichaelBolton/index.html", place => "サンプラザホール" );
	$result[] = array( year => "2010", date => "05/13(木)-15(土)", artist => "Asia", url => "http://t.pia.jp/column/music/asia.html", place => "渋谷C.C.Lemonホール" );
	$result[] = array( year => "2010", date => "05/08(土),09(日)", artist => "DIZZY MIZZ LIZZY", url => "http://eplus.jp/sys/T1U90P006001P0050001P002037678P0030002P0030001P0007P0010413", place => "CLUB CITTA’(神奈川県)" );
	$result[] = array( year => "2010", date => "04/22(木),28(水)", artist => "GARY MOORE", url => "http://www.udo.co.jp/Artist/GaryMoore/index.html", place => "東京国際フォーラムetc" );
	$result[] = array( year => "2010", date => "04/14(水),16(金)", artist => "CAROLE KING / JAMES TAYLOR", url => "http://www.udo.co.jp/Artist/CaroleKingJamesTaylor/index.html", place => "日本武道館" );
	$result[] = array( year => "2010", date => "04/13(火)-16(金)", artist => "GAMMA RAY / RAGE", url => "http://eplus.jp/sys/T1U90P006001P0050001P002002998P0030006P0030007P0007P0010413P0491", place => "渋谷O－EAST" );
	$result[] = array( year => "2010", date => "04/10(土)-13(火)", artist => "Jeff Beck", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P2=012735:P5=0001:P6=001", place => "東京国際フォーラム" );
	$result[] = array( year => "2010", date => "04/04(日)", artist => "PUNK SPRING '10", url => "http://www.punkspring.com/10/", place => "幕張メッセ" );
	$result[] = array( year => "2010", date => "03/25(木),26(金)", artist => "EUROPE", url => "http://eplus.jp/sys/T1U90P006001P0050001P002010807P0030015P0030014P0007P0010413", place => "渋谷O－EAST" );
	$result[] = array( year => "2010", date => "03/21(日)-29(月)", artist => "BOB DYLAN", url => "http://t.pia.jp/feature/music/bobdylan/bobdylan.html", place => "Zepp Tokyo" );
	$result[] = array( year => "2010", date => "03/10(水)", artist => "Backyard Babies", url => "http://okini.eplus.jp/3325/", place => "SHIBUYA－AX" );
	$result[] = array( year => "2010", date => "03/12(金),14(日)", artist => "AC/DC", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11019238", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2010", date => "03/04(木),05(金)", artist => "DRAGONFORCE", url => "http://eplus.jp/sys/T1U90P006001P0050001P002014394P0030023P0007P0010413", place => "渋谷CLUB QUATTRO" );
	$result[] = array( year => "2010", date => "03/02(火)-11(木)", artist => "シェリル・クロウ/ジャクソン・ブラウン", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P5=0001:P2=034625:P6=001", place => "東京国際フォーラム" );
	$result[] = array( year => "2010", date => "02/11(木)-14(日)", artist => "Whitney Houston", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P1=0413:P2=034617:P5=0001:P6=001", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2010", date => "01/23(土),24(日)", artist => "GREEN DAY", url => "http://eplus.jp/sys/T1U14P0010163P0108P002001136P0050001P006001P0030021", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2010", date => "01/22(金),23(土)", artist => "FAIR WARNING", url => "http://eplus.jp/sys/T1U90P006001P0050001P002034493P0030006P0030002P0030007P0030001P0007P0010413", place => "東京厚生年金会館" );
	$result[] = array( year => "2010", date => "01/12(火),13(木)", artist => "Michael Schenker Group", url => "http://eplus.jp/sys/T1U90P006001P0050001P002015665P0030012P0030010P0030011P0007P0010413", place => "中野サンプラザ" );
	$result[] = array( year => "2009", date => "12/19(土)", artist => "Guns n' Roses", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11010530", place => "東京ドーム" );
	$result[] = array( year => "2009", date => "12/07(月)", artist => "Eric Martin", url => "http://artists.pia.jp/pia/artists.do?artistsCd=21240004", place => "オーチャードホール" );
	$result[] = array( year => "2009", date => "10/17(土),18(日)", artist => "LOUD PARK '09", url => "http://www.loudpark.com/09/", place => "幕張メッセ" );
	$result[] = array( year => "2009", date => "09/25(金),26(土)", artist => "THE DOOBIE BROTHERS", url => "http://www.udo.co.jp/Artist/DBDTB/index.html", place => "東京国際フォーラム" );
	$result[] = array( year => "2009", date => "08/13(木)", artist => "Limp Bizkit", url => "http://eplus.jp/sys/T1U14P0010163P0108P002031333P0050001P006001P0030003", place => "Zepp Osaka" );
	$result[] = array( year => "2009", date => "08/07,08,09", artist => "SUMMER SONIC '09", url => "http://www.summersonic.com/", place => "幕張メッセ" );
	$result[] = array( year => "2009", date => "07/24,25,26", artist => "FUJI ROCK FESTIVAL '09", url => "http://t.pia.jp/feature/music/frf/frf.html", place => "苗場スキー場" );
	$result[] = array( year => "2009", date => "06/10(土)", artist => "MR.BIG", url => "http://www.udo.co.jp/Artist/MrBig/index.html", place => "日本武道館" );
	$result[] = array( year => "2009", date => "05/01(金)", artist => "OVER THE RAINBOW", url => "http://artists.pia.jp/pia/artists.do?artistsCd=91230020", place => "東京厚生年金会館" );
	$result[] = array( year => "2009", date => "04/08,15(水)", artist => "Yngwie Malmsteen／Deep Purple", url => "http://artists.pia.jp/pia/artists.do?artistsCd=23200049", place => "東京国際フォーラム" );
	$result[] = array( year => "2009", date => "04/07(火)", artist => "NOFX／BAD RELIGION", url => "http://yo-gaku.eplus2.jp/article/113790991.html", place => "SHIBUYA-AX" );
	$result[] = array( year => "2009", date => "04/05(日)", artist => "PUNK SPRING '09", url => "http://t.pia.jp/sp/punkspring/punkspring.html", place => "幕張メッセ" );
	$result[] = array( year => "2009", date => "04/02(木)", artist => "HOOBASTANK", url => "http://www.udo.co.jp/Artist/Hoobastank/index.html", place => "SHIBUYA-AX" );
	$result[] = array( year => "2009", date => "03/24(火),25(水)", artist => "BECK", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11012000", place => "NHKホール" );
	$result[] = array( year => "2009", date => "03/20(金)", artist => "Oasis", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11010311", place => "幕張メッセ" );
	$result[] = array( year => "2009", date => "03/11,12,14", artist => "Rod Stewart", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11012531", place => "日本武道館/さいたまスーパーアリーナ" );
	$result[] = array( year => "2009", date => "03/09(月)", artist => "JOURNEY", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11012610", place => "東京国際フォーラム" );
	$result[] = array( year => "2009", date => "03/08(日)", artist => "HANOI ROCKS", url => "http://eplus.jp/sys/web/yo-gaku/artist/P000004433.html", place => "新木場STUDIO COAST" );
	$result[] = array( year => "2009", date => "02/21(土),22(日)", artist => "ERIC CLAPTON／JEFF BECK", url => "http://www.udo.co.jp/Artist/ECJB/index.html", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2009", date => "02/15(日)-28(土)", artist => "ERIC CLAPTON", url => "http://www.udo.co.jp/Artist/EricClapton/index.html", place => "日本武道館" );
	$result[] = array( year => "2009", date => "02/14(土),15(日)", artist => "Janet Jackson", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11010975", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2009", date => "02/11(水),12", artist => "COLDPLAY", url => "http://t.pia.jp/sp/coldplay/coldplay.html", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2009", date => "02/06(金),07,09,11", artist => "JEFF BECK", url => "http://www.udo.co.jp/Artist/JeffBeck/index.html", place => "国際フォーラム,NHK,パシフィコ横浜" );
	$result[] = array( year => "2009", date => "02/01(日),02,04,05", artist => "BRIAN SETZER ORCHESTRA", url => "http://www.udo.co.jp/Artist/BSO/index.html", place => "渋谷C.C.Lemonホール" );
	$result[] = array( year => "2009", date => "02/01(日),02(月)", artist => "PAUL GILBERT", url => "http://www.udo.co.jp/Artist/PaulGilbert/index.html", place => "渋谷クラブクアトロ" );
	$result[] = array( year => "2008", date => "12/16(火)-18(木)", artist => "Sting", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11011130&eventPage2=&regionCd=01", place => "オーチャードホール" );
	$result[] = array( year => "2008", date => "12/11(木),12(金)", artist => "Sheryl Crow", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11010900&eventPage2=&regionCd=01", place => "JCB HALL" );
	$result[] = array( year => "2008", date => "12/01(月)-04(木)", artist => "Extreme", url => "http://www.udo.co.jp/artist/Extreme/index.html", place => "渋谷C.C.Lemonホール" );
	$result[] = array( year => "2008", date => "11/18(火)", artist => "BILLY JOEL", url => "http://ent.pia.jp/pia/event.do?eventCd=0828077", place => "東京ドーム" );
	$result[] = array( year => "2008", date => "11/14(金),16,17", artist => "THE WHO", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11021309", place => "横浜アリーナ/さいたまスーパーアリーナ/日本武道館" );
	$result[] = array( year => "2008", date => "10/24(木),25(金)", artist => "DEF LEPPARD／WHITESNAKE", url => "http://ent.pia.jp/pia/event.do?eventCd=0822106", place => "日本武道館" );
	$result[] = array( year => "2008", date => "10/18(土),19(日)", artist => "LOUD PARK '08", url => "http://www.loudpark.com/", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2008", date => "10/04(土),05(日)", artist => "Radiohead", url => "http://ent.pia.jp/pia/event.do?eventCd=0812932", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2008", date => "09/28(日),29(月)", artist => "Judas Priest", url => "http://www.udo.co.jp/artist/JudasPriest/", place => "パシフィコ横浜/日本武道館" );
	$result[] = array( year => "2008", date => "09/16(火)", artist => "Avril Lavigne", url => "http://artists.pia.jp/pia/artists.do?artistsCd=26050069", place => "東京ドーム" );
	$result[] = array( year => "2008", date => "08/09(土),10(日)", artist => "SUMMER SONIC '08", url => "http://search.pia.jp/pia/search_all.do?kw=SUMMER+SONIC+08", place => "幕張メッセ" );
	$result[] = array( year => "2008", date => "07/25(金)-27(日)", artist => "FUJI ROCK FESTIVAL '08", url => "http://t.pia.jp/feature/music/frf/frf.html", place => "苗場スキー場" );
	$result[] = array( year => "2008", date => "07/05(土),06(日)", artist => "Children Of Bodom", url => "http://eplus.jp/sys/web/yo-gaku/artist/P000293758.html", place => "新木場スタジオコースト" );
	$result[] = array( year => "2008", date => "04/24(木)", artist => "CHEAP TRICK", url => "http://ent.pia.jp/pia/event.do?eventCd=0806782", place => "日本武道館" );
	$result[] = array( year => "2008", date => "04/19(土),20(日)", artist => "Night Ranger／Fire House", url => "http://www.udo.co.jp/artist/NRFH/", place => "赤坂BLITZ" );
	$result[] = array( year => "2008", date => "04/19(土),20(日)", artist => "Chicago／HUEY LEWIS and THE NEWS", url => "http://www.udo.co.jp/artist/CH/#tokyo", place => "東京国際フォーラム ホールA" );
	$result[] = array( year => "2008", date => "04/13(日)", artist => "Foo Fighters", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11011883", place => "幕張メッセ" );
	$result[] = array( year => "2008", date => "04/08(火),09(水)", artist => "Rancid", url => "http://ent.pia.jp/pia/event.do?eventCd=0808394", place => "Zepp Tokyo" );
	$result[] = array( year => "2008", date => "04/06(日)", artist => "PUNK SPRING '08", url => "http://search.pia.co.jp/perform.htm?pcd=278225&sheetNo=223443", place => "幕張メッセ" );
	$result[] = array( year => "2008", date => "03/20(木),21(金)", artist => "TOTO／ボズ・スキャッグス", url => "http://search.pia.co.jp/artist.htm?artistId=11011502", place => "JCB HALL" );
	$result[] = array( year => "2008", date => "03/08(土),09(日)", artist => "Celine Dion", url => "http://search.pia.co.jp/artist.htm?artistId=11011203", place => "東京ドーム" );
	$result[] = array( year => "2008", date => "02/19(火),22(金)", artist => "Bjork", url => "http://search.pia.co.jp/artist.htm?artistId=11012641", place => "日本武道館" );
	$result[] = array( year => "2008", date => "02/15(金),16(土)", artist => "IRON MAIDEN", url => "http://search.pia.co.jp/artist.htm?artistId=11010017", place => "パシフィコ横浜/幕張メッセ" );
	$result[] = array( year => "2008", date => "02/14(木),15(金)", artist => "HELLOWEEN", url => "http://eplus.jp/sys/main.jsp?prm=U=14:P5=0001:P2=001370:P6=001:P1=0413", place => "東京厚生年金会館" );
	$result[] = array( year => "2008", date => "02/13(水)", artist => "THE POLICE", url => "http://info.pia.co.jp/et/thepolice/thepolice.jsp", place => "東京ドーム" );
	$result[] = array( year => "2008", date => "02/09(土),10(日)", artist => "Rage Against the Machine", url => "http://search.pia.co.jp/artist.htm?artistId=11012874", place => "幕張メッセ" );
	$result[] = array( year => "2008", date => "02/04(月),05(火)", artist => "HARDCORE SUPERSTAR", url => "http://artists.pia.jp/pia/artists.do?artistsCd=11017314", place => "CLUB QUATTRO" );
	$result[] = array( year => "2008", date => "01/22(火),23(水)", artist => "ザッパ・プレイズ・ザッパ", url => "http://search.pia.co.jp/artist.htm?artistId=79050062", place => "Zepp Tokyo" );
	$result[] = array( year => "2008", date => "01/17(木)", artist => "Kula Shaker", url => "http://search.pia.co.jp/artist.htm?artistId=11019926", place => "SHIBUYA-AX" );
	$result[] = array( year => "2008", date => "01/15(火)", artist => "Dream Theater", url => "http://search.pia.co.jp/artist.htm?artistId=11024772", place => "日本武道館" );
	$result[] = array( year => "2008", date => "01/13(日),14(月)", artist => "BON JOVI", url => "http://www.udo.co.jp/artist/BonJovi/", place => "東京ドーム" );
	$result[] = array( year => "2008", date => "01/06(日)", artist => "Avenged Sevenfold", url => "http://artists.pia.jp/pia/artists.do?artistsCd=63030097", place => "横浜BLITZ" );
	$result[] = array( year => "2007", date => "12/08(土),09(日)", artist => "DAFT PUNK", url => "http://search.pia.co.jp/artist.htm?artistId=11021910", place => "幕張メッセ" );
	$result[] = array( year => "2007", date => "11/29(木),30(金)", artist => "Velvet Revolver", url => "http://search.pia.co.jp/artist.htm?artistId=44230048", place => "横浜BLITZ/日本武道館" );
	$result[] = array( year => "2007", date => "11/23(金),24(土)", artist => "LINKIN PARK", url => "http://eplus.jp/sys/main.jsp?prm=U=88:P0=GGWB01:P10=1:P2=007635:P5=0001:P6=001", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2007", date => "11/20(火)", artist => "エルトン・ジョン", url => "http://search.pia.co.jp/perform.htm?pcd=267118&sheetNo=203549", place => "日本武道館" );
	$result[] = array( year => "2007", date => "10/30(火),31(水)", artist => "MEGADETH", url => "http://www.udo.co.jp/artist/Megadeth/index.html", place => "渋谷C.C.Lemonホール" );
	$result[] = array( year => "2007", date => "10/22(月)", artist => "HANOI ROCKS", url => "http://search.pia.co.jp/artist.htm?artistId=2B020001", place => "CLUB QUATTRO" );
	$result[] = array( year => "2007", date => "10/20(土),21(日)", artist => "LOUD PARK '07", url => "http://www.loudpark.com/", place => "さいたまスーパーアリーナ" );
	$result[] = array( year => "2007", date => "10/04(木),05(金)", artist => "Less Than Jake", url => "http://search.pia.co.jp/artist.htm?artistId=11012501", place => "CLUB QUATTRO（東京都） " );
	$result[] = array( year => "2007", date => "09/26(水),27(木)", artist => "ハーレム・スキャーレム／サイレント・フォース", url => "http://search.pia.co.jp/artist.htm?artistId=11013121", place => "CLUB QUATTRO（東京）" );
	$result[] = array( year => "2007", date => "09/26(水),27(木)", artist => "パット・メセニー／ブラッド・メルドー", url => "http://search.pia.co.jp/perform.htm?pcd=258345&sheetNo=187210", place => "NHKホール" );
	$result[] = array( year => "2007", date => "09/16(日),17(月)", artist => "THUNDER", url => "http://ap0.pia.co.jp/pia/et/onsale/ons_event_list.jsp;jsessionid=0000BDIGzLKW9KcYWeyDAWogVvB:10pc4gd8p", place => "CLUB CITTA'(神奈川),Shibuya O-EAST" );
	$result[] = array( year => "2007", date => "08/11(土),12(日)", artist => "SUMMER SONIC '07", url => "http://www.summersonic.com/index.html", place => "幕張メッセ" );
	$result[] = array( year => "2007", date => "07/27(金)-29(日)", artist => "FUJI ROCK FESTIVAL '07", url => "http://www.fujirockfestival.com/", place => "苗場スキー場" );
	$result[] = array( year => "2007", date => "07/14(土),15(日)", artist => "Guns n' Roses", url => "http://www.creativeman.co.jp/2007/GNR_special/index.html", place => "幕張メッセ" );
	$result[] = array( year => "2007", date => "06/05(火),06(水)", artist => "RED HOT CHILI PEPPERS", url => "http://info.pia.co.jp/et/promo/music/rhcp_info.html", place => "東京ドーム" );
	$result[] = array( year => "2007", date => "05/31(木)", artist => "Queensryche", url => "http://ap1.pia.co.jp/pia/OnsLookup?ONS_PERFORM_KEYWORD=%83N%83C%81%5b%83%93%83Y%83%89%83C%83N&ONS_B_GENRE_CD=01", place => "" );
	$result[] = array( year => "2007", date => "05/30(水)", artist => "アルカトラス／ジョー・リン・ターナー", url => "http://ap1.pia.co.jp/pia/OnsLookup?ONS_PERFORM_KEYWORD=%83W%83%87%81%5b%81E%83%8a%83%93%81E%83%5e%81%5b%83i%81%5b", place => "" );
	$result[] = array( year => "2007", date => "05/09(水)", artist => "Stevie Salas", url => "http://ap1.pia.co.jp/pia/OnsLookup?ONS_PERFORM_KEYWORD=%83N%83C%81%5b%83%93%83Y%83%89%83C%83N&ONS_B_GENRE_CD=01", place => "" );

	if ( $file_max == 'all' || $file_max == 'future' ) {
		$data_cnt = count( $result );
	} else {
		$data_cnt = $file_max;
	}

	$yyyymm = date( Ym );
	for ( $ix = 0 ; $ix < $data_cnt ; $ix++ ) {
		if ( $file_max == 'future' ) {
			$data_ym = $result[$ix]['year'] . substr( $result[$ix]['date'], 0, 2 );
			if ( $data_ym >= $yyyymm ) {
				$new_data[$result[$ix]['year']][] = $result[$ix];
			}
		} else {
			$new_data[$result[$ix]['year']][] = $result[$ix];
		}
	}

	return $new_data;
}
?>
