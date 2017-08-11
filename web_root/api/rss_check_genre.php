<?
/*****************************************************************************
 *  name        : rss_check_genre.php?genre=xx
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
	$ClsRSS = new ClsRSS();

	$genre = $_GET['genre'];
	echo "genre : " . $genre . "<br>";
	if ( isset( $genre ) ) {
		$assign_data = array();
		$assign_data['merge_data'] = $ClsRSS->getFullRSS( $genre, '20' );
		$assign_data['rss_site'] = $rss_site;

	
		echo "<pre>";
		//print_r ( $assign_data['merge_data'] );
		print_r ( $rss_site );
		foreach( $assign_data['merge_data'] as $key => $val ) {
			print( "[".$key."][".$val['update']."]".$val['link_from']." : ".$val['title']."<br>" );
			print( $val['data']."<br>" );
		}
		echo "</pre>";
	}

/**********/
/* 全終了 */
/**********/
	exit();
?>
