<?php
/*****************************************************************************
 *  name        : RSS.inc
 *  function    : RSSクラス
 *
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// RSSクラス
//////////////////////////////////////////////////////////////////////////////
class ClsRSS {

	/******************/
	/* コンストラクタ */
	/******************/
	function ClsRSS() {
		require_once( 'XML/RSS.php' );
	}

	/***********/
	/* RSS取得 */
	/***********/
	function getFullRSS( $genre = 'all', $limit = '20' ) {

		global $rss_site;
		$rss_site = array();	
		if ( $genre == 'music_oversea' ) {
			$rss_site[] = array( 'site' => 'BARKS', 'URL' => 'http://rss.rssad.jp/rss/barks/barks_news_oversea.rdf' );
			$rss_site[] = array( 'site' => 'RO69', 'URL' => 'http://ro69.jp/news/index.rss?category=2' );
		} elseif ( $genre == 'music_item' ) {
			$rss_site[] = array( 'site' => 'RandoM', 'URL' => 'http://rdm.ne.jp/feed/rss' );
			$rss_site[] = array( 'site' => 'Supernice!', 'URL' => 'http://effector-hakase.com/feed' );
			$rss_site[] = array( 'site' => 'Supernice!', 'URL' => 'http://guitar-amp.biz/feed' );
		} elseif ( $genre == 'it_it' ) {
			$rss_site[] = array( 'site' => 'ITpro', 'URL' => 'http://itpro.nikkeibp.co.jp/rss/news.rdf' );
			$rss_site[] = array( 'site' => 'gihyo.jp', 'URL' => 'http://gihyo.jp/feed/rss1' );
			$rss_site[] = array( 'site' => 'TechCrunch', 'URL' => 'http://jp.techcrunch.com/feed/', 'no_desc' => true );
			$rss_site[] = array( 'site' => 'THE BRIDGE', 'URL' => 'http://feeds.feedburner.com/SdJapan?format=xml' );
			$rss_site[] = array( 'site' => 'CNET Japan', 'URL' => 'http://feeds.japan.cnet.com/rss/cnet/all.rdf' );
			$rss_site[] = array( 'site' => 'InfoQ', 'URL' => 'https://www.infoq.com/jp/feed' );
		} elseif ( $genre == 'it_program' ) {
			$rss_site[] = array( 'site' => 'Code Zine', 'URL' => 'http://rss.rssad.jp/rss/codezine/new/20/index.xml' );
		} elseif ( $genre == 'it_infra' ) {
			$rss_site[] = array( 'site' => 'ITpro Cloud', 'URL' => 'http://itpro.nikkeibp.co.jp/rss/cloud.rdf' );
			$rss_site[] = array( 'site' => 'クラウド Watch', 'URL' => 'http://cloud.watch.impress.co.jp/cda/rss/cloud.rdf' );
			$rss_site[] = array( 'site' => 'Think IT', 'URL' => 'http://thinkit.co.jp/rss.xml' );
			$rss_site[] = array( 'site' => 'HEARTBEATS', 'URL' => 'https://heartbeats.jp/hbblog/atom.xml' );
		} elseif ( $genre == 'it_posting' ) {
			//$rss_site[] = array( 'site' => 'はてな', 'URL' => 'http://b.hatena.ne.jp/hotentry/it.rss' );
			//$rss_site[] = array( 'site' => 'Qiita', 'URL' => 'http://qiita.com/popular-items/feed' );
		} elseif ( $genre == 'it_company' ) {
			$rss_site[] = array( 'site' => 'cookpad', 'URL' => 'http://techlife.cookpad.com/rss', 'no_desc' => true );
			$rss_site[] = array( 'site' => 'はてな', 'URL' => 'http://developer.hatenastaff.com/rss', 'no_desc' => true );
			$rss_site[] = array( 'site' => 'mercari', 'URL' => 'http://tech.mercari.com/rss', 'no_desc' => true );
			$rss_site[] = array( 'site' => 'TORETA', 'URL' => 'http://tech.toreta.in/rss', 'no_desc' => true );
			$rss_site[] = array( 'site' => 'LINE', 'URL' => 'http://developers.linecorp.com/blog/ja/?feed=rss2', 'no_desc' => true );
		} elseif ( $genre == 'it_yuru' ) {
			$rss_site[] = array( 'site' => 'Gigazine', 'URL' => 'http://feed.rssad.jp/rss/gigazine/rss_2.0' );
			//$rss_site[] = array( 'site' => 'ネタりか', 'URL' => 'http://netallica.yahoo.co.jp/rss/top' );
			$rss_site[] = array( 'site' => 'ASCII.jp', 'URL' => 'http://rss.rssad.jp/rss/ascii/rss.xml' );
		} elseif ( $genre == 'game' ) {
			$rss_site[] = array( 'site' => 'SocailGameInfo', 'URL' => 'http://gamebiz.jp/?feed=rss' );
			$rss_site[] = array( 'site' => 'GameBusiness.jp', 'URL' => 'http://www.gamebusiness.jp/rss/rss.php' );
			$rss_site[] = array( 'site' => '4Gamer.net', 'URL' => 'http://www.4gamer.net/rss/index.xml' );
			$rss_site[] = array( 'site' => 'ファミ通', 'URL' => 'http://www.famitsu.com/rss/fcom_all.rdf' );
		} elseif ( $genre == 'car' ) {
			$rss_site[] = array( 'site' => 'Carview', 'URL' => 'http://www.carview.co.jp/rss/?t=news' );
			$rss_site[] = array( 'site' => 'オートックワン', 'URL' => 'http://autoc-one.jp/news/rss/' );
		} elseif ( $genre == 'health' ) {
			$rss_site[] = array( 'site' => 'HeatlTech', 'URL' => 'https://healthtechnews.jp/feed/' );
			$rss_site[] = array( 'site' => 'マイナビ', 'URL' => 'http://feeds.journal.mycom.co.jp/rss/mycom/healthcare/mental' );
			$rss_site[] = array( 'site' => '日経', 'URL' => 'http://techon.nikkeibp.co.jp/ndh/index.rdf' );
			$rss_site[] = array( 'site' => 'ITmedia', 'URL' => 'http://headlines.yahoo.co.jp/rss/it_health-c_life.xml' );
		} elseif ( $genre == 'blog' || $genre == 'blog_new' ) {
			$rss_site[] = array( 'site' => 'yoshi111kazu', 'URL' => 'https://blog.ishino.info/?feed=rss2' );
		} else {
			$rss_site[] = array( 'site' => 'BARKS', 'URL' => 'http://rss.rssad.jp/rss/barks/barks_news_oversea.rdf' );
			$rss_site[] = array( 'site' => 'RandoM', 'URL' => 'http://rdm.ne.jp/feed/rss' );
		}

		$rss_data = array();	
		$result = array();

		foreach ( $rss_site as $key => $val ) {
			$rss =& new XML_RSS( $val['URL'] );
			$rss->parse();
			$rss_data = doAuto2Utf8( $rss->getItems() );
			//print_r( $rss_data );
			//debug_print( $rss_data, $val['site'] );
			if ( count( $rss_data ) > 0 ) {
				$rss_cnt = 0;
				foreach ( $rss_data as $key2 => $val2 ) {
					//print_r( $val2 );
					if ( $rss_cnt >= $limit ) {
						break;
					}
					// Atom
					if ( isset( $val2['published'] ) ) {
						$rss_date_tmp = preg_replace( '/T|[\+Z].+/', ' ', $val2['published'] );
						$rss_date = preg_replace( '/-/', '/', $rss_date_tmp );
					// Rss1.0
					} else if ( isset( $val2['dc:date'] ) ) {
						$rss_date_tmp = preg_replace( '/T|[\+Z].+/', ' ', $val2['dc:date'] );
						$rss_date = preg_replace( '/-/', '/', $rss_date_tmp );
					// Rss2.0
					} else if ( isset( $val2['pubdate'] ) ) {
						if ( $val['site'] == 'Think IT' ) {
							$rss_date = preg_replace( '/\(.*\)/', '/', $val2['pubdate'] );
							$rss_date = preg_replace( '/-/', '/', $rss_date );
							$rss_date = preg_replace( '/年/', '/', $rss_date );
							$rss_date = preg_replace( '/月/', '/', $rss_date );
							$rss_date = preg_replace( '/日/', '', $rss_date );
						} else {
							$rss_date = date( 'Y/m/d H:i:s', strtotime( $val2['pubdate'] ) );
						}
					} else {
						$rss_date = '-';
					}
					$rss_date = trim( $rss_date );

					// 広告系以外を取得(AD: PR:)
					if ( isset( $val2['title'] ) ) {
						if ( !preg_match( '/^AD:/', $val2['title'] ) && !preg_match( '/^PR:/', $val2['title'] ) ) {

							//descriptionの<p>タグなどの削除
							/*
							$desc_tmp = '';
							$desc_tmp = preg_replace( '/(?:\n|\r|\r\n)/s', '', $val2['description'] );
							$desc_tmp = preg_replace( '/\<p\>/', '', $desc_tmp );
							$desc_tmp = preg_replace( '/\<\/p\>/', '', $desc_tmp );
							$desc_tmp = preg_replace( '/\<img.*\/\>/', '', $desc_tmp );
							$desc_tmp = preg_replace( '/\<a.*href.*\<\/a\>/', '', $desc_tmp );
							$desc_tmp = preg_replace( '/^　/', '', $desc_tmp );
							$desc_tmp = preg_replace( '/^ /', '', $desc_tmp );
							$desc_tmp = preg_replace( '/\<b\>\<a .*\<\/a\>\<\/b\>/', '', $desc_tmp );
							*/

							//個別処理
							if ( $val['no_desc'] == true ) {
								$val2['description'] = '';
							}
							if ( $val['site'] == 'RO69' ) {
								$val2['title'] = preg_replace( '/ニュース - /', '', $val2['title'] );
							}
							if ( $val['site'] == 'yoshi111kazu' ) {
								$val2['description'] = $val2['content:encoded'];
							}

							$result[] = array( 'title' => $val2['title'],
													'link' => $val2['link'],
													'link_from' => $val['site'],
													'data' =>  $val2['description'],
													'update' => '['.$rss_date.']'
													);
							$rss_cnt++;
						}
					}
				}
			}
		}

		/*
		if ( $genre == 'music' ) {
			$udo_data = $this->GetLiveData_UDO();
			foreach ( $udo_data as $key1 => $val1 ) {
				$result[] = $val1;
			}
		}
		*/

		// ソート
		doArrayUrsort( $result, 'update' );
		//debug_print( $result, "result" );

		return $result;
	}

	/*******************/
	/* Live情報 by UDO */
	/*******************/
	function GetLiveData_UDO() {
		$result = array();

		$url = 'http://www.udo.co.jp/';
		$html = file( $url );

		foreach ( $html as $key1 => $val1 ) {
			if ( preg_match( '/\<div class="news_list"\>/', $val1 ) ) {
				$desc_tmp = preg_replace( '/(?:\n|\r|\r\n)/s', '', $val1 );

				$ary_date = mb_substr( $desc_tmp, mb_strpos( $desc_tmp, 'news_list_date' ) + 16, 10 );
				$ary_date = preg_replace( '/\./', '/', $ary_date );
				$ary_date = '[' . $ary_date . ' 00:00:00]';
				$desc_tmp = mb_strcut( $desc_tmp, mb_strpos( $desc_tmp, 'news_list_date' ) + 33, null );

				$desc_tmp = preg_replace( '/\<div class="news_list"\>/', '', $desc_tmp );
				$desc_tmp = preg_replace( '/\<\/div\>/', '', $desc_tmp );
				$desc_tmp = preg_replace( '/\<span class="news_list_date"\>/', '', $desc_tmp );
				$desc_tmp = preg_replace( '/\<span class="ArtistName"\>/', '', $desc_tmp );
				$desc_tmp = preg_replace( '/\<\/span\>/', '', $desc_tmp );
				$desc_tmp = preg_replace( '/\&gt\;/', '', $desc_tmp );
				$desc_tmp = preg_replace( '/\<a href="/', '<a target="_blank" href="'.$url, $desc_tmp );
				$desc_tmp = trim( $desc_tmp );
				$result[] = array( 'title' => $desc_tmp,
										'link' => '',
										'link_from' => 'UDO',
										'data' =>  $desc_tmp,
										'image_name' => '',
										'image_link' =>  '',
										'update' => $ary_date
										);
			}
		}
		//debug_print( $result, "result" );

		return $result;
	}
}
?>
