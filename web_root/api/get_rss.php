<?
/*****************************************************************************
 *  name        : get_rss.php
 *  function    : 
 *
 ****************************************************************************/
/****************/
/* インクルード */
/****************/
	require_once( '../../src/inc/define.inc' );
	require_once( INC_PHP_PATH."RSS.inc" );

/********/
/* main */
/********/
	$genre = $_GET['genre'];
	$filename = WEB_ROOT . 'api/data/' . $genre . '_rss.json';
	if ( file_exists( $filename ) ) {
		$data = array();
		$data = file_get_contents($filename);

		header("Content-Type: application/json; charset=utf-8");
		echo $data;
	}

/**********/
/* 全終了 */
/**********/
	exit();
?>
