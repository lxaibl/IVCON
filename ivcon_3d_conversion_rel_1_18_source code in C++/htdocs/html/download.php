
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
  <TITLE>IVCon download information</TITLE>
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

<div class="big_hdr" id="titlebar">Download IVCon</div>

<div id="toplogo"><img <?php echo "src=\"".$home_path."/pictures/ivcon_icon_64.png\""; ?>
 ALT="IVCon logo" TITLE="IVCon logo"/></div>

<?php drawSidebar('sidebar_downld'); ?>

<div id="page_content">

<p>You can download <b>IVCon</b> at
<a href="https://sourceforge.net/project/showfiles.php?group_id=191003">SourceForge
 IVCon releases</a> page</p>

<p>Packages explanation:<br>
  <ul>
  <li><b>ivcon_3d_conversion_rel_x_xx_win32</b> contains Windows compiled version and readme</li>
  <li><b>ivcon_3d_conversion_rel_x_xx_src</b> contains source code in C++ and logo pictures</li>
  <li><b>ivcon_3d_conversion_rel_x_xx_smpl</b> contains documentation and samples</li>
  </ul>
</p>

<hr>

<p>If you wish to have the latest source code and compile IVCon by yourself,
you can download CVS repository hosted by SourceForge.</p>

<p>To do this, you must have CVS installed on your system. From a command line, type:</p>

<pre>
cvs -d:pserver:anonymous@ivcon-tl.cvs.sourceforge.net:/cvsroot/ivcon-tl login 
cvs -z3 -d:pserver:anonymous@ivcon-tl.cvs.sourceforge.net:/cvsroot/ivcon-tl co -P src
</pre>

<p>The repository will be stored in the current directory on your disk.</p>

<p>The available modules are:</p>

<pre>
src    contains source code of IVCon
site   contains WWW site of the project
</pre>

<hr>

<?php addGlobalFooterTags(); ?>

</div>

<BR>

</BODY>
</HTML>
 