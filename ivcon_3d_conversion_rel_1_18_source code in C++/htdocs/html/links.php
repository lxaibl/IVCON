
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
  <TITLE>IVCon related links</TITLE>
  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">

<?php
   require_once '../include/basics.php';
   $home_path="../";
   addGlobalHeaderTags();
?>

</HEAD>

<?php
   require_once $home_path.'include/sidebar.php';
?>

<BODY TOPMARGIN=0 LEFTMARGIN=0 MARGINWIDTH=0 MARGINHEIGHT=0>

<div class="big_hdr" id="titlebar">IVCon related links</div>

<div id="toplogo"><img <?php echo "src=\"".$home_path."/pictures/ivcon_icon_64.png\""; ?>
 ALT="IVCon logo" TITLE="IVCon logo"/></div>

<?php drawSidebar('sidebar_links'); ?>

<div id="page_content">

<p><a href="http://people.scs.fsu.edu/~burkardt/cpp_src/ivcon/ivcon.html">
John Burkardt's IVCon page</a> - site where you can download unmodified John Burkardt
version of IVCon, abd read about supported file formats. For every format,
John published many example files.</p>

<h3 align="center">
Book references:
</h3>

<ol>
  <li>
  Adrian Bowyer and John Woodwark,<br>
  A Programmer's Geometry,<br>
  Butterworths, 1983.
  </li>
  <li>
  James Foley, Andries van Dam, Steven Feiner, John Hughes,<br>
  Computer Graphics, Principles and Practice,<br>
  Addison Wesley, Second Edition, 1990.
  </li>
  <li>
  3D Systems, Inc,<br>
  Stereolithography Interface Specification,<br>
  October 1989.
  </li>
</ol>

<hr>
<?php addGlobalFooterTags(); ?>

</div>

<BR>

</BODY>
</HTML>
 