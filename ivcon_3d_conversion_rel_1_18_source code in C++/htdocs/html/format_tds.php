
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
  <TITLE>Graphics formats supported by IVCon</TITLE>
  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">

<?php
   require_once '../include/basics.php';
   $home_path="../";
   $file_ext="3DS";
   addGlobalHeaderTags();
?>

</HEAD>

<?php
   require_once $home_path.'include/sidebar.php';
?>

<BODY TOPMARGIN=0 LEFTMARGIN=0 MARGINWIDTH=0 MARGINHEIGHT=0>

<div class="big_hdr" id="titlebar">Supported 3D formats</div>

<div id="toplogo"><img <?php echo "src=\"".$home_path."/pictures/ivcon_icon_64.png\""; ?>
 ALT="IVCon logo" TITLE="IVCon logo"/></div>

<?php drawSidebar('sidebar_formt','format_tds'); ?>

<div id="page_content">

<h3><?php echo $file_ext; ?> format overview:</h3>
<p>
  3DS format was created by Autodesk for 3D Studio. It has become very popular,
   and is now supported by many programs. Also, the new versions of Autodesk's
   3D Studio Max can still support this old format.<br>
  The format is now quite obsolete, because it is 16-bit based, but it is still
   widely used. Note that this format has not changed since its very beginnings,
   so there are no version conflicts with these files.
</p>
<hr>

<h3><?php echo $file_ext; ?> file characteristics:</h3>
<p>
  <ul>
  <li>binary</li>
  <li>3D vertices data</li>
  <li>Faces and meshes definitions</li>
  <li>Lines definitions</li>
  <li>Cameras and lights definitions</li>
  <li>Color and textured</li>
  <li>No compression</li>
  </ul>
</p>
<hr>

<h3><?php echo $file_ext; ?> format references and information:</h3>
<p>
  <a href="http://people.scs.fsu.edu/%7Eburkardt/data/3ds/3ds.html">
  3DS format information and examples</a> from John Burkardt
</p>
<hr>

<h3><?php echo $file_ext; ?> support notes inside IVCon source code:</h3>
<p>
<?php printCodeComment($home_path."/src/tds.cpp",256); ?>
</p>

<hr>
<?php addGlobalFooterTags(); ?>

</div>

<BR>

</BODY>
</HTML>
 