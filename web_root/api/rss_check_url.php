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
$msg = <<< EOF
<form method="POST" action="" method="post">
  URL: <input type=text name="url"></text>
 <input type="submit" value="送信">
</form>
EOF;
echo $msg;
echo "<hr>";

	$url = $_POST['url'];
	if ( isset( $url ) ) {
		$ClsRSS = new ClsRSS();
		$rss =& new XML_RSS( $url );
		$rss->parse();
		$rss_data = doAuto2Utf8( $rss->getItems() );
		echo "URL: " . $url . "<br><br>";
		echo '<font size="-1">';
		print_r ( $rss_data );
		echo "</font>";
	}

/**********/
/* 全終了 */
/**********/
	exit();
?>
