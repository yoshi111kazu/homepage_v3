<?php
/*****************************************************************************
 *  name        : Blog.inc
 *  function    : Blogクラス
 *
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// Blogクラス
//////////////////////////////////////////////////////////////////////////////
class ClsBlog {

	/******************/
	/* コンストラクタ */
	/******************/
	function ClsBlog() {
	}

	/***********/
	/* Blog取得 */
	/***********/
	function getBlog() {

		require_once( 'Services/Blogging.php' );

		$result = array();

		$blogObj = Services_Blogging::discoverDriver( BLOGGER_HOST, BLOGGER_USER, BLOGGER_PASS );
		$result = $blogObj->getBlogs();  

		print_r( $result );

		return $result;
	}
}
?>
