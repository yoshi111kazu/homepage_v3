<?
/*****************************************************************************
 *  name        : put_rss.php
 *  function    : 
 *
 ****************************************************************************/
/****************/
/* インクルード */
/****************/
	require_once( 'define.inc' );
	require_once( INC_PHP_PATH."RSS.inc" );

/********/
/* main */
/********/
	$filepath = WEB_ROOT . 'api/data/';
	$ClsRSS = new ClsRSS();

	$genre = array( 'music_oversea', 'music_item', 'it_it', 'it_program', 'it_infra', 'it_posting', 'it_company', 'it_yuru', 'game', 'car', 'health', 'blog', 'blog_new' );
	foreach( $genre as $key => $val ) {
		$assign_data = array();
		if ( $val == 'blog_new' ) {
			$data_limit = 5;
		} else {
			$data_limit = 20;
		}
		$assign_data['merge_data'] = $ClsRSS->getFullRSS( $val, $data_limit );
		$assign_data['rss_site'] = $rss_site;

		if ( count( $assign_data['merge_data'] ) > 0 ) {
			$filename = $val . '_rss.json';
			$fp = fopen( $filepath . $filename, 'w+' );
			fwrite($fp, json_encode($assign_data['merge_data']) );
			fclose($fp);
		}
	}

/**********/
/* 全終了 */
/**********/
	exit();
?>
