FasdUAS 1.101.10   ��   ��    k             l      ��  ��   +% 
 *	AEOpenFile
 *	chris piuggi 2014
 *	
 *	Script take one argument (as string) filepath to new file to open in After Effects
 *	If no files are provided the script returns an error.
 *	
 *	Running the Script from CLI
 *	
 *	$ osascript AEOpenFile.scpt '/full/path/to/my/AE/template.aep'
 *
      � 	 	J   
   * 	 A E O p e n F i l e 
   * 	 c h r i s   p i u g g i   2 0 1 4 
   * 	 
   * 	 S c r i p t   t a k e   o n e   a r g u m e n t   ( a s   s t r i n g )   f i l e p a t h   t o   n e w   f i l e   t o   o p e n   i n   A f t e r   E f f e c t s 
   * 	 I f   n o   f i l e s   a r e   p r o v i d e d   t h e   s c r i p t   r e t u r n s   a n   e r r o r . 
   * 	 
   * 	 R u n n i n g   t h e   S c r i p t   f r o m   C L I 
   * 	 
   * 	 $   o s a s c r i p t   A E O p e n F i l e . s c p t   ' / f u l l / p a t h / t o / m y / A E / t e m p l a t e . a e p ' 
   * 
     
  
 l     ��������  ��  ��     ��  i         I     �� ��
�� .aevtoappnull  �   � ****  o      ���� 0 argv  ��    Z       ��   ?         l     ����  I    �� ��
�� .corecnte****       ****  o     ���� 0 argv  ��  ��  ��    m    ����    k   
        l  
 
��  ��     return (item 1 of argv)     �   . r e t u r n   ( i t e m   1   o f   a r g v )   ��  O   
     I   ��  ��
�� .aevtodocnull  �    alis   l    !���� ! n     " # " 4    �� $
�� 
cobj $ m    ����  # o    ���� 0 argv  ��  ��  ��    m   
  % %(                                                                                  FXTC  alis    �  Macintosh HD               �0ڲH+  ��Adobe After Effects CC.app                                     �,�!W�        ����  	                Adobe After Effects CC    �1�      �!�;    ���B  MMacintosh HD:Applications: Adobe After Effects CC: Adobe After Effects CC.app   6  A d o b e   A f t e r   E f f e c t s   C C . a p p    M a c i n t o s h   H D  >Applications/Adobe After Effects CC/Adobe After Effects CC.app  / ��  ��  ��    L     & & m     ' ' � ( ( 0 E r r o r :   N o   A r g s   S u b m i t t e d��       �� ) *��   ) ��
�� .aevtoappnull  �   � **** * �� ���� + ,��
�� .aevtoappnull  �   � ****�� 0 argv  ��   + ���� 0 argv   , �� %���� '
�� .corecnte****       ****
�� 
cobj
�� .aevtodocnull  �    alis�� �j  j � 
��k/j UY �ascr  ��ޭ