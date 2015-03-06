<?php

function drawSidebar($active_button='',$active_sub='')
{
  global $home_path;
  echo "<div id=\"sidebar\"><ul id=\"sidelist\">\n";

  if ($active_button=="sidebar_home")
    drawSidebarButtonA($home_path,"IVCon home","sidebar_home");
  else
    drawSidebarButton($home_path,"IVCon home","sidebar_home");

  if ($active_button=="sidebar_usage")
    drawSidebarButtonA($home_path."html/usage.php","How to use","sidebar_usage");
  else
    drawSidebarButton($home_path."html/usage.php","How to use","sidebar_usage");

  if ($active_button=="sidebar_techinfo")
  {
    drawSidebarButtonA($home_path."html/techinfo.php","Technical information","sidebar_techinfo");
  echo "  <center><ul>\n";
    if ($active_sub=="techinfo_concpt")
      drawSidebarSubButtonA($home_path."html/techinfo_concpt.php","Conceptions","techinfo_concpt");
    else
      drawSidebarSubButton($home_path."html/techinfo_concpt.php","Conceptions","techinfo_concpt");
    if ($active_sub=="techinfo_files")
      drawSidebarSubButtonA($home_path."html/techinfo_files.php","Files","techinfo_files");
    else
      drawSidebarSubButton($home_path."html/techinfo_files.php","Files","techinfo_files");
    if ($active_sub=="techinfo_funct")
      drawSidebarSubButtonA($home_path."html/techinfo_funct.php","Methods","techinfo_funct");
    else
      drawSidebarSubButton($home_path."html/techinfo_funct.php","Methods","techinfo_funct");
  echo "  </ul></center>\n";
  }
  else
    drawSidebarButton($home_path."html/techinfo.php","Technical information","sidebar_techinfo");

  if ($active_button=="sidebar_formt")
  {
    drawSidebarButtonA($home_path."html/formats.php","3D formats","sidebar_formt");
  echo "  <center><ul>\n";
    if ($active_sub=="format_tds")
      drawSidebarSubButtonA($home_path."html/format_tds.php","3DS","format_tds");
    else
      drawSidebarSubButton($home_path."html/format_tds.php","3DS","format_tds");
    if ($active_sub=="format_asc")
      drawSidebarSubButtonA($home_path."html/format_asc.php","ASC","format_asc");
    else
      drawSidebarSubButton($home_path."html/format_asc.php","ASC","format_asc");
    if ($active_sub=="format_ase")
      drawSidebarSubButtonA($home_path."html/format_ase.php","ASE","format_ase");
    else
      drawSidebarSubButton($home_path."html/format_ase.php","ASE","format_ase");
    if ($active_sub=="format_byu")
      drawSidebarSubButtonA($home_path."html/format_byu.php","BYU","format_byu");
    else
      drawSidebarSubButton($home_path."html/format_byu.php","BYU","format_byu");
    if ($active_sub=="format_dxf")
      drawSidebarSubButtonA($home_path."html/format_dxf.php","DXF","format_dxf");
    else
      drawSidebarSubButton($home_path."html/format_dxf.php","DXF","format_dxf");
    if ($active_sub=="format_gmod")
      drawSidebarSubButtonA($home_path."html/format_gmod.php","GMOD","format_gmod");
    else
      drawSidebarSubButton($home_path."html/format_gmod.php","GMOD","format_gmod");
    if ($active_sub=="format_hrc")
      drawSidebarSubButtonA($home_path."html/format_hrc.php","HRC","format_hrc");
    else
      drawSidebarSubButton($home_path."html/format_hrc.php","HRC","format_hrc");
    if ($active_sub=="format_iv")
      drawSidebarSubButtonA($home_path."html/format_iv.php","IV","format_iv");
    else
      drawSidebarSubButton($home_path."html/format_iv.php","IV","format_iv");
    if ($active_sub=="format_obj")
      drawSidebarSubButtonA($home_path."html/format_obj.php","OBJ","format_obj");
    else
      drawSidebarSubButton($home_path."html/format_obj.php","OBJ","format_obj");
    if ($active_sub=="format_off")
      drawSidebarSubButtonA($home_path."html/format_off.php","OFF","format_off");
    else
      drawSidebarSubButton($home_path."html/format_off.php","OFF","format_off");
    if ($active_sub=="format_pov")
      drawSidebarSubButtonA($home_path."html/format_pov.php","POV","format_pov");
    else
      drawSidebarSubButton($home_path."html/format_pov.php","POV","format_pov");
    if ($active_sub=="format_smf")
      drawSidebarSubButtonA($home_path."html/format_smf.php","SMF","format_smf");
    else
      drawSidebarSubButton($home_path."html/format_smf.php","SMF","format_smf");
    if ($active_sub=="format_stla")
      drawSidebarSubButtonA($home_path."html/format_stla.php","STLA","format_stla");
    else
      drawSidebarSubButton($home_path."html/format_stla.php","STLA","format_stla");
    if ($active_sub=="format_stlb")
      drawSidebarSubButtonA($home_path."html/format_stlb.php","STLB","format_stlb");
    else
      drawSidebarSubButton($home_path."html/format_stlb.php","STLB","format_stlb");
    if ($active_sub=="format_swm")
      drawSidebarSubButtonA($home_path."html/format_swm.php","SWM","format_swm");
    else
      drawSidebarSubButton($home_path."html/format_swm.php","SWM","format_swm");
    if ($active_sub=="format_swv")
      drawSidebarSubButtonA($home_path."html/format_swv.php","SWV","format_swv");
    else
      drawSidebarSubButton($home_path."html/format_swv.php","SWV","format_swv");
    if ($active_sub=="format_tec")
      drawSidebarSubButtonA($home_path."html/format_tec.php","TEC","format_tec");
    else
      drawSidebarSubButton($home_path."html/format_tec.php","TEC","format_tec");
    if ($active_sub=="format_tria")
      drawSidebarSubButtonA($home_path."html/format_tria.php","TRIA","format_tria");
    else
      drawSidebarSubButton($home_path."html/format_tria.php","TRIA","format_tria");
    if ($active_sub=="format_trib")
      drawSidebarSubButtonA($home_path."html/format_trib.php","TRIB","format_trib");
    else
      drawSidebarSubButton($home_path."html/format_trib.php","TRIB","format_trib");
    if ($active_sub=="format_txt")
      drawSidebarSubButtonA($home_path."html/format_txt.php","TXT","format_txt");
    else
      drawSidebarSubButton($home_path."html/format_txt.php","TXT","format_txt");
    if ($active_sub=="format_ucd")
      drawSidebarSubButtonA($home_path."html/format_ucd.php","UCD","format_ucd");
    else
      drawSidebarSubButton($home_path."html/format_ucd.php","UCD","format_ucd");
    if ($active_sub=="format_vla")
      drawSidebarSubButtonA($home_path."html/format_vla.php","VLA","format_vla");
    else
      drawSidebarSubButton($home_path."html/format_vla.php","VLA","format_vla");
    if ($active_sub=="format_wrl")
      drawSidebarSubButtonA($home_path."html/format_wrl.php","WRL","format_wrl");
    else
      drawSidebarSubButton($home_path."html/format_wrl.php","WRL","format_wrl");
    if ($active_sub=="format_xgl")
      drawSidebarSubButtonA($home_path."html/format_xgl.php","XGL","format_xgl");
    else
      drawSidebarSubButton($home_path."html/format_xgl.php","XGL","format_xgl");
  echo "  </ul></center>\n";
  }
  else
    drawSidebarButton($home_path."html/formats.php","3D formats","sidebar_formt");

  if ($active_button=="sidebar_downld")
  {
    drawSidebarButtonA($home_path."html/download.php","Download","sidebar_downld");
  }
  else
    drawSidebarButton($home_path."html/download.php","Download","sidebar_downld");

  if ($active_button=="sidebar_authrs")
  {
    drawSidebarButtonA($home_path."html/authors.php","Authors","sidebar_authrs");
  }
  else
    drawSidebarButton($home_path."html/authors.php","Authors","sidebar_authrs");

  if ($active_button=="sidebar_links")
  {
    drawSidebarButtonA($home_path."html/links.php","Links","sidebar_links");
  }
  else
    drawSidebarButton($home_path."html/links.php","Links","sidebar_links");

  echo "</ul></div>\n";
}

function drawSidebarButton($target,$text,$buttonID)
{
  echo "  <li class=\"buttn_wide1\"><a href=\"$target\">\n";
  echo "  $text";
  echo "  </a></li>\n";
}

function drawSidebarButtonA($target,$text,$buttonID)
{
  echo "  <li class=\"buttn_wide1_a\"><a href=\"$target\">\n";
  echo "  $text";
  echo "  </a></li>\n";
}

function drawSidebarSubButton($target,$text,$buttonID)
{
  echo "  <li class=\"buttn_med2\"><a href=\"$target\">\n";
  echo "  $text";
  echo "  </a></li>\n";
}

function drawSidebarSubButtonA($target,$text,$buttonID)
{
  echo "  <li class=\"buttn_med2_a\"><a href=\"$target\">\n";
  echo "  $text";
  echo "  </a></li>\n";
}

?>
