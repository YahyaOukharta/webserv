<?PHP
 if (isset($_POST ["file"]))
 {
   $FileName = $File = $_POST ["file"];
 }
 else
 {
   $FileName = "";
 }
?>

<HTML>
<BODY BGCOLOR=#FFFFFF>

<FORM ACTION=/cgi-bin/cgiwrap/username/Example.php METHOD=POST>

<P>Enter FileName:
<?PHP
  echo " <INPUT TYPE=TEXT NAME=file VALUE=\"$FileName\" SIZE=75>\n";
?>
<P><BR><P>
<INPUT TYPE=SUBMIT VALUE="Show Me!">

</FORM>

<P><BR><P>
<P><BR><P>
<P><BR><P>

<?PHP
 if (isset($File))
 {
   $Handle = @fopen ($File, "r"); // '@' suppresses external errors

   if ($Handle)
   {
     $FileText = fread ($Handle, 10000); // Read up to 10,000 Bytes

     fclose ($Handle);

     // Fix HTML tags that may be there

     $SafeText1 = str_replace ("&", "&amp;", $FileText);
     $SafeText2 = str_replace ("<", "&lt;", $SafeText1);
     $SafeText  = str_replace (">", "&gt;", $SafeText2);

     // Now it is safe to display it

     echo " <H2 ALIGN=CENTER>File: $File</H2>\n";

     echo "<PRE>\n";
     echo $SafeText;
     echo "</PRE>\n";
   }
   else
   {
     echo " <H3>Error: File '$File' is not accessible.</H3>\n";
   }
 }
 $i = 0;
 while ($i < 100) {
  echo 'e2r2p6% ls
Cgi.cpp         README.md       configs         parse_conf.cpp  src             test_nginx.sh
Makefile        cfg             main.cpp        post.py         test.rb         webserv
MimeTypes.list  cgi             main2.cpp       siege.urls      test.sh         www
e2r2p6% cd cgi 
e2r2p6% make re && ./a.out
rm -f *.hpp.gch
rm -rf *.dSYM
rm -f a.out
rm -f *.out
clang++ -o a.out -std=c++98 -fsanitize=address -g       -Wall -Wextra -Werror main.cpp 
Got some data from pipe : Content-type:text/html


<html>
<head>
<title>Hello - Second CGI Program</title>
</head>
<body>
<h2>Hello hakim nassif</h2>
</body>
</html>

e2r2p6% which php-cgi
php-cgi not found
e2r2p6% ';
  $i++;
 }

?>

</BODY>