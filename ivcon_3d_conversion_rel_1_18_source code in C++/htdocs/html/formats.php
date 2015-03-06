
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
  <TITLE>Graphics formats supported by IVCon</TITLE>
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

<div class="big_hdr" id="titlebar">Supported 3D formats</div>

<div id="toplogo"><img <?php echo "src=\"".$home_path."/pictures/ivcon_icon_64.png\""; ?>
 ALT="IVCon logo" TITLE="IVCon logo"/></div>

<?php drawSidebar('sidebar_formt'); ?>

<div id="page_content">

<h4>
The supported formats include:
</h4>
<ul>
<li>
  <a href="format_tds.php">3DS</a> 
  AutoCAD and 3D Studio Max binary mesh files;
</li>
<li>
  <a href="format_asc.php">ASC</a> 
  ASC 3D points cloud files;
</li>
<li>
  <a href="format_ase.php">ASE</a> 
  AutoCAD ASCII export files;
</li>
<li>
  <a href="format_byu.php">BYU</a>
  Movie.BYU surface geometry files;
</li>
<li>
  <a href="format_dxf.php">DXF</a> 
  AutoCAD DXF files;
</li>
<li>
  <a href="format_gmod.php">GMOD</a> 
  Golgotha GMOD files;
</li>
<li>
  <a href="format_hrc.php">HRC</a> 
  SoftImage hierarchical files;
</li>
<li>
  <a href="format_iv.php">IV</a> 
  SGI Inventor files;
</li>
<li>
  <a href="format_obj.php">OBJ</a>
  WaveFront Advanced Visualizer ASCII files; a file format from Alias;
</li>
<li>
  <a href="format_off.php">OFF</a> 
  GEOMVIEW OFF files;
</li>
<li>
  <a href="format_pov.php">POV</a> 
  Persistence of Vision files (output only);
</li>
<li>
  <a href="format_smf.php">SMF</a> 
  Michael Garland's format for his QSLIM program;
</li>
<li>
  <a href="format_stla.php">STL/STLA</a>
  ASCII Stereolithography files;
</li>
<li>
  <a href="format_stlb.php">STLB</a>
  binary Stereolithography files;
</li>
<li>
  <a href="format_swm.php">SWM</a>
  Syndicate Wars game maps and structures file format (input only);
</li>
<li>
  <a href="format_swv.php">SWV</a>
  Syndicate Wars game vehicles primveh file format (input only);
</li>
<li>
  <a href="format_tec.php">TEC</a>
  TECPLOT files (output only);
</li>
<li>
  <a href="format_tria.php">TRI/TRIA</a>
  a simple ASCII triangle format requested by Greg Hood;
</li>
<li>
  <a href="format_trib.php">TRIB</a>
  a simple binary triangle format requested by Greg Hood;
</li>
<li>
  <a href="format_txt.php">TXT</a>
  a text dump (output only);
</li>
<li>
  <a href="format_ucd.php">UCD</a>
  Advanced Visual Systems (AVS) Unstructured Cell Data 
  (output only);
</li>
<li>
  <a href="format_vla.php">VLA</a>
  Evans and Sutherland
  Digistar II VLA files for planetariums;
</li>
<li>
  <a href="format_wrl.php">WRL</a>
  WRL/VRML (Virtual Reality Modeling Language) files (output only).
</li>
<li>
  <a href="format_xgl.php">XGL</a>
  the XGL format, based on the XML language and OpenGl graphics 
  (output only).
</li>
</ul>

<hr>
<?php addGlobalFooterTags(); ?>

</div>

<BR><BR>

</BODY>
</HTML>
 