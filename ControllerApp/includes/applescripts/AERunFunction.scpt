FasdUAS 1.101.10   ��   ��    k             l      ��  ��   ��  AERunFunction
 *  chris piuggi 2014
 *
 *
 *  Applescript which takes two arguments jsx file path & jsx function to call. Passing both the file and init function allow us to pass data directly into AE from shell/bash. 
 *  The data contained in the init function can then be parsed and used by the included file afterwards.
 *  
 *	Init script via shell/bash sample:
 * 	$ osascript AERunFunction.scpt "~/Documents/Adobe Scripts/args.jsx" "main('information')"
 *
 *  Script takes the incoming argv and produces jsx to run in AfterEffects(ExtendScript) which looks like this: 
 *  #include '~/Documents/Adobe Scripts/args.jsx'; main('information');
 * 
      � 	 	      A E R u n F u n c t i o n 
   *     c h r i s   p i u g g i   2 0 1 4 
   * 
   * 
   *     A p p l e s c r i p t   w h i c h   t a k e s   t w o   a r g u m e n t s   j s x   f i l e   p a t h   &   j s x   f u n c t i o n   t o   c a l l .   P a s s i n g   b o t h   t h e   f i l e   a n d   i n i t   f u n c t i o n   a l l o w   u s   t o   p a s s   d a t a   d i r e c t l y   i n t o   A E   f r o m   s h e l l / b a s h .   
   *     T h e   d a t a   c o n t a i n e d   i n   t h e   i n i t   f u n c t i o n   c a n   t h e n   b e   p a r s e d   a n d   u s e d   b y   t h e   i n c l u d e d   f i l e   a f t e r w a r d s . 
   *     
   * 	 I n i t   s c r i p t   v i a   s h e l l / b a s h   s a m p l e : 
   *   	 $   o s a s c r i p t   A E R u n F u n c t i o n . s c p t   " ~ / D o c u m e n t s / A d o b e   S c r i p t s / a r g s . j s x "   " m a i n ( ' i n f o r m a t i o n ' ) " 
   * 
   *     S c r i p t   t a k e s   t h e   i n c o m i n g   a r g v   a n d   p r o d u c e s   j s x   t o   r u n   i n   A f t e r E f f e c t s ( E x t e n d S c r i p t )   w h i c h   l o o k s   l i k e   t h i s :   
   *     # i n c l u d e   ' ~ / D o c u m e n t s / A d o b e   S c r i p t s / a r g s . j s x ' ;   m a i n ( ' i n f o r m a t i o n ' ) ; 
   *   
     
�� 
 i         I     �� ��
�� .aevtoappnull  �   � ****  o      ���� 0 argv  ��    k     8       l     ��  ��    ' !check if we have argv (arguments)     �   B c h e c k   i f   w e   h a v e   a r g v   ( a r g u m e n t s )   ��  Z     8  ��   ?         l     ����  I    �� ��
�� .corecnte****       ****  o     ���� 0 argv  ��  ��  ��    m    ����    k   
 3       l  
 
��   ��    &  set our args to a string of 'js'      � ! ! @ s e t   o u r   a r g s   t o   a   s t r i n g   o f   ' j s '   " # " r   
  $ % $ b   
  & ' & b   
  ( ) ( b   
  * + * b   
  , - , b   
  . / . m   
  0 0 � 1 1  # i n c l u d e   ' / l    2���� 2 n     3 4 3 4    �� 5
�� 
cobj 5 m    ����  4 o    ���� 0 argv  ��  ��   - m     6 6 � 7 7  ' ;   + l    8���� 8 n     9 : 9 4    �� ;
�� 
cobj ; m    ����  : o    ���� 0 argv  ��  ��   ) m     < < � = =  ;   ' o    ��
�� 
ret  % o      ���� 0 js   #  > ? > l   ��������  ��  ��   ?  @�� @ O    3 A B A k   " 2 C C  D E D l  " "�� F G��   F - 'instruct After Effects to Run Js Script    G � H H N i n s t r u c t   A f t e r   E f f e c t s   t o   R u n   J s   S c r i p t E  I J I r   " ) K L K I  " '�� M��
�� .miscdoscnull���     ctxt M o   " #���� 0 js  ��   L o      ���� 0 resp   J  N O N l  * *�� P Q��   P  set resp to the result    Q � R R , s e t   r e s p   t o   t h e   r e s u l t O  S T S l  * *�� U V��   U   return js # for debug only    V � W W 4 r e t u r n   j s   #   f o r   d e b u g   o n l y T  X Y X l  * *�� Z [��   Z ! return resp #for production    [ � \ \ 6 r e t u r n   r e s p   # f o r   p r o d u c t i o n Y  ]�� ] L   * 2 ^ ^ b   * 1 _ ` _ b   * / a b a b   * - c d c m   * + e e � f f 
 R e s p : d o   + ,���� 0 resp   b m   - . g g � h h *   S u c c e s s :   R a n   S c r i p t   ` o   / 0���� 0 js  ��   B m     i i(                                                                                  FXTC  alis    �  Macintosh HD               �0ڲH+  ��Adobe After Effects CC.app                                     �,�!W�        ����  	                Adobe After Effects CC    �1�      �!�;    ���B  MMacintosh HD:Applications: Adobe After Effects CC: Adobe After Effects CC.app   6  A d o b e   A f t e r   E f f e c t s   C C . a p p    M a c i n t o s h   H D  >Applications/Adobe After Effects CC/Adobe After Effects CC.app  / ��  ��  ��    L   6 8 j j m   6 7 k k � l l 0 E r r o r :   N o   A r g s   S u b m i t t e d��  ��       �� m n o p����   m ��������
�� .aevtoappnull  �   � ****�� 0 js  �� 0 resp  ��   n �� ���� q r��
�� .aevtoappnull  �   � ****�� 0 argv  ��   q ���� 0 argv   r �� 0�� 6 <���� i���� e g k
�� .corecnte****       ****
�� 
cobj
�� 
ret �� 0 js  
�� .miscdoscnull���     ctxt�� 0 resp  �� 9�j  j .��k/%�%��l/%�%�%E�O� �j E�O��%�%�%UY � o � s s # i n c l u d e   ' / U s e r s / c h r i s / o F / 0 8 0 / a p p s / 3 0 P P / C o n t r o l l e r A p p / m o d u l e s / . . / i n c l u d e s / a e s c r i p t s / q u i t W i t h o u t S a v i n g . j s x ' ;   q u i t W i t h o u t S a v i n g ( ) ;    p � t t  0��   ascr  ��ޭ