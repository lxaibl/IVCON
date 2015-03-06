<?php

   global $home_path;
   $home_path="./";

function getFileSize($file)
{
   $size = filesize($file);
   for ($si = 0; $size >= 1024; $size /= 1024, $si++);
   return round($size, 1) . ' ' . trim(substr(' KMGT', $si, 1)) . 'B';
}

function addGlobalHeaderTags()
{
   global $home_path;
   echo "  <META NAME=\"Author\" CONTENT=\"Tomasz Lis\">\n";
   echo "  <META NAME=\"Language\" CONTENT=\"English\">\n";
   echo "  <META NAME=\"Keywords\" CONTENT=\"ivcon 3d graphic file format convert conversion converting 3ds ase byu dxf gmod hrc iv obj off pov smf stl stla stlb tec tri tria trib ucd vla wrl vrml xgl site\">\n";
   echo "  <link rel=\"shortcut icon\" href=\"".$home_path."favicon.ico\" />\n";
   echo "  <LINK REL=STYLESHEET TYPE=\"text/css\" HREF=\"".$home_path."sitestyle/site_style.css\">\n";
   echo "  <LINK REL=STYLESHEET TYPE=\"text/css\" HREF=\"".$home_path."sitestyle/ivcon_style.css\">\n";
}

function addGlobalFooterTags()
{
   global $home_path;
   echo "<div id=\"sflogo\"><a href=\"http://sourceforge.net\"><img\n";
   echo " src=\"http://sflogo.sourceforge.net/sflogo.php?group_id=191003&amp;type=3\"\n";
   echo " width=\"125\" height=\"37\" border=\"0\" alt=\"SourceForge.net Logo\" /></a></div>\n";
}

function drawSiteButton($target,$text,$type="buttn_wide1",$buttonID="")
{
  echo "  <div class=\"$type\"><a href=\"$target\">\n";
  echo "  <div name=\"$buttonID\">$text</div>";
  echo "  </a></div>\n";
}

function drawSiteButtonA($target,$text,$type="buttn_wide1_a",$buttonID="")
{
  echo "  <div class=\"$type\"><a href=\"$target\">\n";
  echo "  <div name=\"$buttonID\">$text</div>";
  echo "  </a></div>\n";
}

function printCodeComment($filename,$max_lines=16777216,$name_attr='')
{
    $fd = fopen ($filename, "r");
    if ($fd===FALSE)
    {
      echo "Sorry, information currently not available.\n";
      return;
    }
    while (!feof($fd))
    {
       $buffer = fgets($fd, 4096);
       $lines[] = $buffer;
    }
    fclose ($fd);

    $num_lines = count ($lines);
    if ($num_lines>$max_lines)
        $num_lines=$max_lines;

    echo "<PRE" . (empty($name_attr) ? '' : " name=\"$name_attr$num_str\"").">";
    $i=0;

    $connmt_s="/*********";
    $connmt_l=strlen($connmt_s);
    while($i < $num_lines)
    {
       if (strncmp($lines[$i],$connmt_s,$connmt_l)==0)
           break 1;
       $i++;
    }
    $i++;
    while($i < $num_lines)
    {
       $to_print=trim($lines[$i]);
       if (strncmp($to_print,$connmt_s,$connmt_l)==0)
           break 1;
       if ( (strncmp($to_print,"/*",2)==0) ||
            (strncmp($to_print,"*/",2)==0) ||
            (strncmp($to_print,"//",2)==0) )
         $to_print=substr($to_print,2);
       echo htmlspecialchars($to_print)."\n";
       $i++;
    }
    echo "</PRE>";

}

function printMethodsComments($filename,$class_name,$max_lines=16777216)
{
    $fd = fopen ($filename, "r");
    if ($fd===FALSE)
    {
      echo "Sorry, information currently not available.\n";
      return;
    }
    while (!feof($fd))
    {
       $buffer = fgets($fd, 4096);
       $lines[] = $buffer;
    }
    fclose ($fd);

    $num_lines = count ($lines);
    if ($num_lines>$max_lines)
        $num_lines=$max_lines;

    $i=0;

    $class_df="class ".$class_name;
    while($i < $num_lines)
    {
       $to_check=$lines[$i];
       $clss_df_pos=strpos($to_check,$class_df);
       if ($clss_df_pos!==FALSE) 
       {
           $cmnt_df_pos=strpos($to_check,"//");
           if ($cmnt_df_pos===FALSE)
              $cmnt_df_pos=512;
           if ($clss_df_pos<$cmnt_df_pos)
               break 1;
       }
       $i++;
    }
    $i++;
    if ($i>=$num_lines)
    {
        echo "Sorry, class definition not found.\n";
        return;
    }
    echo "<ul>\n";
    $act_commnt="";
    $act_func="none";
    while ($i<$num_lines)
    {
       $to_print=trim($lines[$i]);
       if (strncmp($to_print,"//",2)==0)
         $act_commnt=substr($to_print,2);
       else
       {
         if ( (strlen($to_print)>6) &&
              (strncmp($to_print,"/*",2)!=0) )
         {
         if (strpos($to_print,":")===FALSE)
           {
             $cmnt_pos=strpos($to_print,"//");
             if ($cmnt_pos===FALSE)
                $cmnt_pos=512;
             $act_func=substr($to_print,0,$cmnt_pos);
             $act_func=trim(substr($act_func,0,strpos($act_func,"(")));
             $act_func=substr($act_func,strrpos($act_func," "));
             if (strlen($act_func)>1)
               echo "<li><b>".htmlspecialchars(strtoupper($act_func))."</b> ".
                      htmlspecialchars($act_commnt)."</li>\n";
           } else
           {
             echo "</ul>\n";
             echo "<ul>\n";
           }
         }
         $act_commnt="";
       }
       $i++;
    }
    echo "</ul>\n";

}



?>
