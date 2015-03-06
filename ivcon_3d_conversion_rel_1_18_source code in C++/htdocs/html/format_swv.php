
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
  <TITLE>Graphics formats supported by IVCon</TITLE>
  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">

<?php
   require_once '../include/basics.php';
   $home_path="../";
   $file_ext="SWV";
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

<?php drawSidebar('sidebar_formt','format_swv'); ?>

<div id="page_content">

<h3><?php echo $file_ext; ?> format overview:</h3>
<p>
  SWV is just an extension implemented to make support of the file
   format inside IVCon. This file extension is not the original one
   used for storing vehicles in Syndicate Wars.<br>
  Syndicate Wars is a game developed by Bullfrog and published in
   1997. It is a RTS game, probably the first PC games with fully 3D
   world. Inside the game world, buildings and vehicles are simple 3D
   models.<br>
  There is one SWV file inside Syndicate Wars - its original name is
   PRIMVEH.OBJ. This file contains definitions of Syndicate Wars cars,
   and after renaming to .SWV, can be read by IVCon.
</p>
<hr>

<h3><?php echo $file_ext; ?> file characteristics:</h3>
<p>
  <ul>
  <li>binary</li>
  <li>3D vertices data</li>
  <li>Faces and meshes definition</li>
  <li>No lines definition</li>
  <li>No cameras or lights definition</li>
  <li>Textured materials</li>
  <li>No compression</li>
  </ul>
</p>
<hr>

<h3><?php echo $file_ext; ?> format references and information:</h3>
<p>
  <a href="http://syndicate.lubie.org/">
  Syndicate Series Unofficial Fan Site</a> by Tomasz Lis
</p>
<hr>

<h3><?php echo $file_ext; ?> support notes inside IVCon source code:</h3>
<p>
<?php printCodeComment($home_path."/src/swv.cpp",256); ?>
</p>

<hr>
<?php addGlobalFooterTags(); ?>

</div>

<BR>

</BODY>
</HTML>
 