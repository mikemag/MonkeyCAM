( Sample-splitboard-nose-tail-spacers.nc                                       )
( Generated by MonkeyCAM v4.0.7 master@2f43be8                                 )
(   at 2019-04-20 12:50:28 PDT                                                 )
(                                                                              )
( This GCode program is provided on an "AS IS" BASIS WITHOUT WARRANTIES OR     )
( CONDITIONS OF ANY KIND, either express or implied. You should inspect this   )
( program very carefully to confirm you understand its operation. YOU ARE      )
( RESPONSIBLE FOR ENSURING THIS PROGRAM WILL NOT DAMAGE YOUR MACHINE OR        )
( MATERIALS.                                                                   )
(                                                                              )
( See http://www.github.com/mikemag/MonkeyCAM for License and documentation.   )
(                                                                              )
( * Rapid height: 0.2500" [0.6350cm]                                           )
( * Tool: T1, Quarter Inch Straight, diameter 0.2500" [0.6350cm]               )
( * Height baseline [Z=0.0] is the top of the table.                           )
( * XY origin is the left side of the table, Y is center of the part.          )
( * Requires G54 to be the part work coordinate offsets [WCO]. [0, 0] is the   )
(   center of the nose, with the board extending to positive X. The cutter may )
(   move to negative values of X.                                              )
( * Requires G55 to be the machine WCO, with Z above the rapid height.         )
( * Units are inches                                                           )
( * Center of the board in G54:                                                )
(     X=33.8582 Y=0.0000 Z=0.0000                                              )
(                                                                              )
( Spacer material length: 15.3858" [39.0800cm]                                 )
(                                                                              )
( * Cutter bounding box in G54:                                                )
(     X0.0000 Y-6.2600 Z-0.0100 to X13.7233 Y6.2600 Z0.2500                    )

G90 G20 G17 G40 G49
G43 H1 T1
G00 G54 X0 Y0 Z0.2500

G00 X8.2116 Y-6.2600 Z0.2500
M03
G04 P5
G01 X8.2116 Y-6.2600 Z-0.0100 F80
G01 X8.2114 Y-6.1678 Z-0.0100
G01 X8.2110 Y-6.0798 Z-0.0100
G01 X8.2110 Y-6.0792 Z-0.0100
G01 X8.2103 Y-5.9961 Z-0.0100
G01 X8.2103 Y-5.9953 Z-0.0100
G01 X8.2091 Y-5.9165 Z-0.0100
G01 X8.2090 Y-5.9157 Z-0.0100
G01 X8.2074 Y-5.8411 Z-0.0100
G01 X8.2074 Y-5.8399 Z-0.0100
G01 X8.2051 Y-5.7694 Z-0.0100
G01 X8.2051 Y-5.7680 Z-0.0100
G01 X8.2023 Y-5.7014 Z-0.0100
G01 X8.2022 Y-5.6997 Z-0.0100
G01 X8.1987 Y-5.6370 Z-0.0100
G01 X8.1986 Y-5.6350 Z-0.0100
G01 X8.1944 Y-5.5760 Z-0.0100
G01 X8.1942 Y-5.5736 Z-0.0100
G01 X8.1891 Y-5.5182 Z-0.0100
G01 X8.1888 Y-5.5153 Z-0.0100
G01 X8.1829 Y-5.4634 Z-0.0100
G01 X8.1825 Y-5.4600 Z-0.0100
G01 X8.1756 Y-5.4114 Z-0.0100
G01 X8.1750 Y-5.4075 Z-0.0100
G01 X8.1670 Y-5.3622 Z-0.0100
G01 X8.1662 Y-5.3577 Z-0.0100
G01 X8.1572 Y-5.3154 Z-0.0100
G01 X8.1560 Y-5.3102 Z-0.0100
G01 X8.1459 Y-5.2709 Z-0.0100
G01 X8.1441 Y-5.2650 Z-0.0100
G01 X8.1328 Y-5.2285 Z-0.0100
G01 X8.1305 Y-5.2219 Z-0.0100
G01 X8.1179 Y-5.1882 Z-0.0100
G01 X8.1148 Y-5.1807 Z-0.0100
G01 X8.1009 Y-5.1497 Z-0.0100
G01 X8.0970 Y-5.1418 Z-0.0100
G01 X8.0816 Y-5.1133 Z-0.0100
G01 X8.0766 Y-5.1048 Z-0.0100
G01 X8.0598 Y-5.0787 Z-0.0100
G01 X8.0537 Y-5.0701 Z-0.0100
G01 X8.0353 Y-5.0462 Z-0.0100
G01 X8.0284 Y-5.0379 Z-0.0100
G01 X8.0085 Y-5.0161 Z-0.0100
G01 X8.0005 Y-5.0082 Z-0.0100
G01 X7.9788 Y-4.9885 Z-0.0100
G01 X7.9705 Y-4.9815 Z-0.0100
G01 X7.9472 Y-4.9637 Z-0.0100
G01 X7.9386 Y-4.9577 Z-0.0100
G01 X7.9134 Y-4.9415 Z-0.0100
G01 X7.9048 Y-4.9366 Z-0.0100
G01 X7.8777 Y-4.9221 Z-0.0100
G01 X7.8698 Y-4.9182 Z-0.0100
G01 X7.8408 Y-4.9053 Z-0.0100
G01 X7.8335 Y-4.9023 Z-0.0100
G01 X7.8024 Y-4.8907 Z-0.0100
G01 X7.7958 Y-4.8885 Z-0.0100
G01 X7.7627 Y-4.8783 Z-0.0100
G01 X7.7570 Y-4.8766 Z-0.0100
G01 X7.7217 Y-4.8675 Z-0.0100
G01 X7.7168 Y-4.8664 Z-0.0100
G01 X7.6793 Y-4.8583 Z-0.0100
G01 X7.6752 Y-4.8575 Z-0.0100
G01 X7.6355 Y-4.8503 Z-0.0100
G01 X7.6319 Y-4.8497 Z-0.0100
G01 X7.5899 Y-4.8434 Z-0.0100
G01 X7.5872 Y-4.8430 Z-0.0100
G01 X7.5427 Y-4.8373 Z-0.0100
G01 X7.5403 Y-4.8370 Z-0.0100
G01 X7.4934 Y-4.8319 Z-0.0100
G01 X7.4917 Y-4.8317 Z-0.0100
G01 X7.4423 Y-4.8270 Z-0.0100
G01 X7.4409 Y-4.8269 Z-0.0100
G01 X7.3888 Y-4.8225 Z-0.0100
G01 X7.3879 Y-4.8224 Z-0.0100
G01 X7.3331 Y-4.8182 Z-0.0100
G01 X7.3326 Y-4.8181 Z-0.0100
G01 X7.2751 Y-4.8140 Z-0.0100
G01 X7.2145 Y-4.8098 Z-0.0100
G01 X7.1514 Y-4.8053 Z-0.0100
G01 X7.0853 Y-4.8007 Z-0.0100
G01 X7.0168 Y-4.7955 Z-0.0100
G01 X6.9458 Y-4.7900 Z-0.0100
G01 X6.8711 Y-4.7837 Z-0.0100
G01 X6.7933 Y-4.7766 Z-0.0100
G01 X6.7120 Y-4.7686 Z-0.0100
G01 X6.6291 Y-4.7597 Z-0.0100
G01 X6.5407 Y-4.7494 Z-0.0100
G01 X6.4528 Y-4.7385 Z-0.0100
G01 X6.2105 Y-4.7005 Z-0.0100
G01 X5.9785 Y-4.6577 Z-0.0100
G01 X5.7536 Y-4.6096 Z-0.0100
G01 X5.5389 Y-4.5574 Z-0.0100
G01 X5.3291 Y-4.4998 Z-0.0100
G01 X5.1278 Y-4.4378 Z-0.0100
G01 X4.9374 Y-4.3727 Z-0.0100
G01 X4.7530 Y-4.3031 Z-0.0100
G01 X4.5756 Y-4.2294 Z-0.0100
G01 X4.4074 Y-4.1529 Z-0.0100
G01 X4.2438 Y-4.0717 Z-0.0100
G01 X4.0893 Y-3.9882 Z-0.0100
G01 X3.9410 Y-3.9012 Z-0.0100
G01 X3.7992 Y-3.8111 Z-0.0100
G01 X3.6638 Y-3.7180 Z-0.0100
G01 X3.5354 Y-3.6227 Z-0.0100
G01 X3.4125 Y-3.5242 Z-0.0100
G01 X3.2953 Y-3.4229 Z-0.0100
G01 X3.1847 Y-3.3199 Z-0.0100
G01 X3.0788 Y-3.2137 Z-0.0100
G01 X2.9784 Y-3.1050 Z-0.0100
G01 X2.8839 Y-2.9948 Z-0.0100
G01 X2.7931 Y-2.8808 Z-0.0100
G01 X2.7078 Y-2.7651 Z-0.0100
G01 X2.6275 Y-2.6472 Z-0.0100
G01 X2.5522 Y-2.5278 Z-0.0100
G01 X2.4804 Y-2.4044 Z-0.0100
G01 X2.4137 Y-2.2800 Z-0.0100
G01 X2.3520 Y-2.1544 Z-0.0100
G01 X2.2936 Y-2.0246 Z-0.0100
G01 X2.2404 Y-1.8949 Z-0.0100
G01 X2.1910 Y-1.7621 Z-0.0100
G01 X2.1455 Y-1.6268 Z-0.0100
G01 X2.1043 Y-1.4901 Z-0.0100
G01 X2.0671 Y-1.3513 Z-0.0100
G01 X2.0344 Y-1.2126 Z-0.0100
G01 X2.0057 Y-1.0724 Z-0.0100
G01 X1.9805 Y-0.9283 Z-0.0100
G01 X1.9596 Y-0.7848 Z-0.0100
G01 X1.9425 Y-0.6400 Z-0.0100
G01 X1.9292 Y-0.4925 Z-0.0100
G01 X1.9199 Y-0.3467 Z-0.0100
G01 X1.9143 Y-0.1973 Z-0.0100
G01 X1.9124 Y-0.0484 Z-0.0100
G01 X1.9124 Y0.0484 Z-0.0100
G01 X1.9143 Y0.1973 Z-0.0100
G01 X1.9199 Y0.3467 Z-0.0100
G01 X1.9292 Y0.4925 Z-0.0100
G01 X1.9425 Y0.6400 Z-0.0100
G01 X1.9595 Y0.7842 Z-0.0100
G01 X1.9805 Y0.9283 Z-0.0100
G01 X2.0057 Y1.0724 Z-0.0100
G01 X2.0344 Y1.2126 Z-0.0100
G01 X2.0671 Y1.3513 Z-0.0100
G01 X2.1043 Y1.4901 Z-0.0100
G01 X2.1455 Y1.6268 Z-0.0100
G01 X2.1910 Y1.7621 Z-0.0100
G01 X2.2404 Y1.8949 Z-0.0100
G01 X2.2936 Y2.0246 Z-0.0100
G01 X2.3520 Y2.1544 Z-0.0100
G01 X2.4137 Y2.2800 Z-0.0100
G01 X2.4804 Y2.4044 Z-0.0100
G01 X2.5522 Y2.5278 Z-0.0100
G01 X2.6275 Y2.6472 Z-0.0100
G01 X2.7078 Y2.7651 Z-0.0100
G01 X2.7931 Y2.8808 Z-0.0100
G01 X2.8839 Y2.9948 Z-0.0100
G01 X2.9784 Y3.1050 Z-0.0100
G01 X3.0788 Y3.2137 Z-0.0100
G01 X3.1847 Y3.3199 Z-0.0100
G01 X3.2950 Y3.4227 Z-0.0100
G01 X3.4125 Y3.5242 Z-0.0100
G01 X3.5354 Y3.6227 Z-0.0100
G01 X3.6638 Y3.7180 Z-0.0100
G01 X3.7992 Y3.8111 Z-0.0100
G01 X3.9410 Y3.9012 Z-0.0100
G01 X4.0893 Y3.9882 Z-0.0100
G01 X4.2438 Y4.0717 Z-0.0100
G01 X4.4074 Y4.1529 Z-0.0100
G01 X4.5756 Y4.2294 Z-0.0100
G01 X4.7530 Y4.3031 Z-0.0100
G01 X4.9374 Y4.3727 Z-0.0100
G01 X5.1278 Y4.4378 Z-0.0100
G01 X5.3291 Y4.4998 Z-0.0100
G01 X5.5389 Y4.5574 Z-0.0100
G01 X5.7536 Y4.6096 Z-0.0100
G01 X5.9785 Y4.6577 Z-0.0100
G01 X6.2105 Y4.7005 Z-0.0100
G01 X6.4528 Y4.7385 Z-0.0100
G01 X6.5407 Y4.7494 Z-0.0100
G01 X6.6291 Y4.7597 Z-0.0100
G01 X6.7120 Y4.7686 Z-0.0100
G01 X6.7933 Y4.7766 Z-0.0100
G01 X6.8711 Y4.7837 Z-0.0100
G01 X6.9458 Y4.7900 Z-0.0100
G01 X7.0168 Y4.7955 Z-0.0100
G01 X7.0853 Y4.8007 Z-0.0100
G01 X7.1514 Y4.8053 Z-0.0100
G01 X7.2145 Y4.8098 Z-0.0100
G01 X7.2748 Y4.8140 Z-0.0100
G01 X7.3326 Y4.8181 Z-0.0100
G01 X7.3331 Y4.8182 Z-0.0100
G01 X7.3879 Y4.8224 Z-0.0100
G01 X7.3888 Y4.8225 Z-0.0100
G01 X7.4409 Y4.8269 Z-0.0100
G01 X7.4423 Y4.8270 Z-0.0100
G01 X7.4917 Y4.8317 Z-0.0100
G01 X7.4934 Y4.8319 Z-0.0100
G01 X7.5403 Y4.8370 Z-0.0100
G01 X7.5427 Y4.8373 Z-0.0100
G01 X7.5872 Y4.8430 Z-0.0100
G01 X7.5899 Y4.8434 Z-0.0100
G01 X7.6319 Y4.8497 Z-0.0100
G01 X7.6355 Y4.8503 Z-0.0100
G01 X7.6752 Y4.8575 Z-0.0100
G01 X7.6793 Y4.8583 Z-0.0100
G01 X7.7168 Y4.8664 Z-0.0100
G01 X7.7217 Y4.8675 Z-0.0100
G01 X7.7570 Y4.8766 Z-0.0100
G01 X7.7627 Y4.8783 Z-0.0100
G01 X7.7958 Y4.8885 Z-0.0100
G01 X7.8024 Y4.8907 Z-0.0100
G01 X7.8335 Y4.9023 Z-0.0100
G01 X7.8408 Y4.9053 Z-0.0100
G01 X7.8698 Y4.9182 Z-0.0100
G01 X7.8777 Y4.9221 Z-0.0100
G01 X7.9048 Y4.9366 Z-0.0100
G01 X7.9134 Y4.9415 Z-0.0100
G01 X7.9386 Y4.9577 Z-0.0100
G01 X7.9472 Y4.9637 Z-0.0100
G01 X7.9705 Y4.9815 Z-0.0100
G01 X7.9788 Y4.9885 Z-0.0100
G01 X8.0005 Y5.0082 Z-0.0100
G01 X8.0085 Y5.0161 Z-0.0100
G01 X8.0284 Y5.0379 Z-0.0100
G01 X8.0353 Y5.0462 Z-0.0100
G01 X8.0537 Y5.0701 Z-0.0100
G01 X8.0598 Y5.0787 Z-0.0100
G01 X8.0766 Y5.1048 Z-0.0100
G01 X8.0816 Y5.1133 Z-0.0100
G01 X8.0970 Y5.1418 Z-0.0100
G01 X8.1009 Y5.1497 Z-0.0100
G01 X8.1148 Y5.1807 Z-0.0100
G01 X8.1179 Y5.1882 Z-0.0100
G01 X8.1305 Y5.2219 Z-0.0100
G01 X8.1328 Y5.2285 Z-0.0100
G01 X8.1441 Y5.2650 Z-0.0100
G01 X8.1459 Y5.2709 Z-0.0100
G01 X8.1560 Y5.3102 Z-0.0100
G01 X8.1572 Y5.3154 Z-0.0100
G01 X8.1662 Y5.3577 Z-0.0100
G01 X8.1670 Y5.3622 Z-0.0100
G01 X8.1750 Y5.4075 Z-0.0100
G01 X8.1756 Y5.4114 Z-0.0100
G01 X8.1825 Y5.4600 Z-0.0100
G01 X8.1829 Y5.4634 Z-0.0100
G01 X8.1888 Y5.5153 Z-0.0100
G01 X8.1891 Y5.5182 Z-0.0100
G01 X8.1942 Y5.5736 Z-0.0100
G01 X8.1944 Y5.5760 Z-0.0100
G01 X8.1986 Y5.6350 Z-0.0100
G01 X8.1987 Y5.6370 Z-0.0100
G01 X8.2022 Y5.6997 Z-0.0100
G01 X8.2023 Y5.7014 Z-0.0100
G01 X8.2051 Y5.7680 Z-0.0100
G01 X8.2051 Y5.7694 Z-0.0100
G01 X8.2074 Y5.8399 Z-0.0100
G01 X8.2074 Y5.8411 Z-0.0100
G01 X8.2090 Y5.9157 Z-0.0100
G01 X8.2091 Y5.9165 Z-0.0100
G01 X8.2103 Y5.9953 Z-0.0100
G01 X8.2103 Y5.9961 Z-0.0100
G01 X8.2110 Y6.0792 Z-0.0100
G01 X8.2110 Y6.0798 Z-0.0100
G01 X8.2114 Y6.1674 Z-0.0100
G01 X8.2116 Y6.2600 Z-0.0100
G00 X8.2116 Y6.2600 Z0.2500
G00 X8.9990 Y6.1814 Z0.2500
G01 X8.9990 Y6.1814 Z-0.0100
G01 X8.9990 Y6.0893 Z-0.0100
G01 X8.9993 Y6.0016 Z-0.0100
G01 X8.9998 Y5.9185 Z-0.0100
G01 X8.9998 Y5.9179 Z-0.0100
G01 X9.0007 Y5.8398 Z-0.0100
G01 X9.0007 Y5.8390 Z-0.0100
G01 X9.0019 Y5.7651 Z-0.0100
G01 X9.0020 Y5.7642 Z-0.0100
G01 X9.0037 Y5.6946 Z-0.0100
G01 X9.0037 Y5.6934 Z-0.0100
G01 X9.0060 Y5.6278 Z-0.0100
G01 X9.0060 Y5.6262 Z-0.0100
G01 X9.0089 Y5.5646 Z-0.0100
G01 X9.0090 Y5.5628 Z-0.0100
G01 X9.0125 Y5.5050 Z-0.0100
G01 X9.0127 Y5.5029 Z-0.0100
G01 X9.0170 Y5.4487 Z-0.0100
G01 X9.0172 Y5.4459 Z-0.0100
G01 X9.0223 Y5.3953 Z-0.0100
G01 X9.0226 Y5.3921 Z-0.0100
G01 X9.0286 Y5.3448 Z-0.0100
G01 X9.0291 Y5.3412 Z-0.0100
G01 X9.0360 Y5.2972 Z-0.0100
G01 X9.0368 Y5.2927 Z-0.0100
G01 X9.0448 Y5.2517 Z-0.0100
G01 X9.0459 Y5.2466 Z-0.0100
G01 X9.0549 Y5.2087 Z-0.0100
G01 X9.0564 Y5.2029 Z-0.0100
G01 X9.0666 Y5.1678 Z-0.0100
G01 X9.0687 Y5.1611 Z-0.0100
G01 X9.0801 Y5.1287 Z-0.0100
G01 X9.0830 Y5.1212 Z-0.0100
G01 X9.0957 Y5.0914 Z-0.0100
G01 X9.0994 Y5.0833 Z-0.0100
G01 X9.1135 Y5.0558 Z-0.0100
G01 X9.1183 Y5.0473 Z-0.0100
G01 X9.1338 Y5.0222 Z-0.0100
G01 X9.1396 Y5.0135 Z-0.0100
G01 X9.1565 Y4.9905 Z-0.0100
G01 X9.1635 Y4.9819 Z-0.0100
G01 X9.1820 Y4.9609 Z-0.0100
G01 X9.1896 Y4.9530 Z-0.0100
G01 X9.2098 Y4.9338 Z-0.0100
G01 X9.2182 Y4.9266 Z-0.0100
G01 X9.2401 Y4.9092 Z-0.0100
G01 X9.2482 Y4.9033 Z-0.0100
G01 X9.2718 Y4.8875 Z-0.0100
G01 X9.2801 Y4.8824 Z-0.0100
G01 X9.3055 Y4.8681 Z-0.0100
G01 X9.3133 Y4.8640 Z-0.0100
G01 X9.3407 Y4.8511 Z-0.0100
G01 X9.3475 Y4.8481 Z-0.0100
G01 X9.3769 Y4.8362 Z-0.0100
G01 X9.3830 Y4.8340 Z-0.0100
G01 X9.4144 Y4.8232 Z-0.0100
G01 X9.4197 Y4.8215 Z-0.0100
G01 X9.4532 Y4.8116 Z-0.0100
G01 X9.4577 Y4.8104 Z-0.0100
G01 X9.4934 Y4.8013 Z-0.0100
G01 X9.4970 Y4.8004 Z-0.0100
G01 X9.5350 Y4.7920 Z-0.0100
G01 X9.5379 Y4.7914 Z-0.0100
G01 X9.5781 Y4.7834 Z-0.0100
G01 X9.5805 Y4.7829 Z-0.0100
G01 X9.6232 Y4.7753 Z-0.0100
G01 X9.6250 Y4.7750 Z-0.0100
G01 X9.6701 Y4.7676 Z-0.0100
G01 X9.6713 Y4.7675 Z-0.0100
G01 X9.7189 Y4.7601 Z-0.0100
G01 X9.7198 Y4.7600 Z-0.0100
G01 X9.7700 Y4.7527 Z-0.0100
G01 X9.8234 Y4.7450 Z-0.0100
G01 X9.8792 Y4.7371 Z-0.0100
G01 X9.9371 Y4.7288 Z-0.0100
G01 X9.9979 Y4.7200 Z-0.0100
G01 X10.0621 Y4.7103 Z-0.0100
G01 X10.1280 Y4.7000 Z-0.0100
G01 X10.1980 Y4.6884 Z-0.0100
G01 X10.2692 Y4.6761 Z-0.0100
G01 X10.3453 Y4.6622 Z-0.0100
G01 X10.4235 Y4.6471 Z-0.0100
G01 X10.5068 Y4.6301 Z-0.0100
G01 X10.5908 Y4.6120 Z-0.0100
G01 X10.6787 Y4.5919 Z-0.0100
G01 X10.8428 Y4.5475 Z-0.0100
G01 X11.0002 Y4.4989 Z-0.0100
G01 X11.1513 Y4.4462 Z-0.0100
G01 X11.2953 Y4.3901 Z-0.0100
G01 X11.4345 Y4.3298 Z-0.0100
G01 X11.5694 Y4.2651 Z-0.0100
G01 X11.6964 Y4.1983 Z-0.0100
G01 X11.8199 Y4.1271 Z-0.0100
G01 X11.9385 Y4.0524 Z-0.0100
G01 X12.0507 Y3.9756 Z-0.0100
G01 X12.1601 Y3.8942 Z-0.0100
G01 X12.2636 Y3.8108 Z-0.0100
G01 X12.3636 Y3.7236 Z-0.0100
G01 X12.4590 Y3.6338 Z-0.0100
G01 X12.5515 Y3.5398 Z-0.0100
G01 X12.6379 Y3.4451 Z-0.0100
G01 X12.7231 Y3.3445 Z-0.0100
G01 X12.8036 Y3.2420 Z-0.0100
G01 X12.8801 Y3.1372 Z-0.0100
G01 X12.9534 Y3.0288 Z-0.0100
G01 X13.0237 Y2.9168 Z-0.0100
G01 X13.0899 Y2.8030 Z-0.0100
G01 X13.1523 Y2.6869 Z-0.0100
G01 X13.2123 Y2.5662 Z-0.0100
G01 X13.2691 Y2.4424 Z-0.0100
G01 X13.3215 Y2.3183 Z-0.0100
G01 X13.3711 Y2.1903 Z-0.0100
G01 X13.4170 Y2.0610 Z-0.0100
G01 X13.4601 Y1.9278 Z-0.0100
G01 X13.4997 Y1.7931 Z-0.0100
G01 X13.5361 Y1.6556 Z-0.0100
G01 X13.5690 Y1.5175 Z-0.0100
G01 X13.5990 Y1.3751 Z-0.0100
G01 X13.6252 Y1.2341 Z-0.0100
G01 X13.6487 Y1.0890 Z-0.0100
G01 X13.6685 Y0.9450 Z-0.0100
G01 X13.6857 Y0.7959 Z-0.0100
G01 X13.6990 Y0.6522 Z-0.0100
G01 X13.7098 Y0.5003 Z-0.0100
G01 X13.7173 Y0.3515 Z-0.0100
G01 X13.7218 Y0.1996 Z-0.0100
G01 X13.7233 Y0.0487 Z-0.0100
G01 X13.7233 Y-0.0500 Z-0.0100
G01 X13.7218 Y-0.1996 Z-0.0100
G01 X13.7173 Y-0.3519 Z-0.0100
G01 X13.7098 Y-0.5003 Z-0.0100
G01 X13.6989 Y-0.6538 Z-0.0100
G01 X13.6857 Y-0.7961 Z-0.0100
G01 X13.6685 Y-0.9450 Z-0.0100
G01 X13.6487 Y-1.0890 Z-0.0100
G01 X13.6252 Y-1.2341 Z-0.0100
G01 X13.5990 Y-1.3751 Z-0.0100
G01 X13.5690 Y-1.5175 Z-0.0100
G01 X13.5361 Y-1.6556 Z-0.0100
G01 X13.4997 Y-1.7931 Z-0.0100
G01 X13.4601 Y-1.9278 Z-0.0100
G01 X13.4170 Y-2.0610 Z-0.0100
G01 X13.3711 Y-2.1903 Z-0.0100
G01 X13.3215 Y-2.3183 Z-0.0100
G01 X13.2691 Y-2.4424 Z-0.0100
G01 X13.2123 Y-2.5662 Z-0.0100
G01 X13.1523 Y-2.6869 Z-0.0100
G01 X13.0899 Y-2.8030 Z-0.0100
G01 X13.0237 Y-2.9168 Z-0.0100
G01 X12.9534 Y-3.0288 Z-0.0100
G01 X12.8801 Y-3.1372 Z-0.0100
G01 X12.8036 Y-3.2420 Z-0.0100
G01 X12.7231 Y-3.3445 Z-0.0100
G01 X12.6379 Y-3.4451 Z-0.0100
G01 X12.5515 Y-3.5398 Z-0.0100
G01 X12.4590 Y-3.6338 Z-0.0100
G01 X12.3636 Y-3.7236 Z-0.0100
G01 X12.2636 Y-3.8108 Z-0.0100
G01 X12.1601 Y-3.8942 Z-0.0100
G01 X12.0507 Y-3.9756 Z-0.0100
G01 X11.9385 Y-4.0524 Z-0.0100
G01 X11.8199 Y-4.1271 Z-0.0100
G01 X11.6964 Y-4.1983 Z-0.0100
G01 X11.5694 Y-4.2651 Z-0.0100
G01 X11.4345 Y-4.3298 Z-0.0100
G01 X11.2953 Y-4.3901 Z-0.0100
G01 X11.1513 Y-4.4462 Z-0.0100
G01 X11.0002 Y-4.4989 Z-0.0100
G01 X10.8428 Y-4.5475 Z-0.0100
G01 X10.6787 Y-4.5919 Z-0.0100
G01 X10.5908 Y-4.6119 Z-0.0100
G01 X10.5068 Y-4.6301 Z-0.0100
G01 X10.4235 Y-4.6471 Z-0.0100
G01 X10.3453 Y-4.6622 Z-0.0100
G01 X10.2693 Y-4.6761 Z-0.0100
G01 X10.1980 Y-4.6885 Z-0.0100
G01 X10.1276 Y-4.7000 Z-0.0100
G01 X10.0621 Y-4.7103 Z-0.0100
G01 X9.9979 Y-4.7200 Z-0.0100
G01 X9.9371 Y-4.7288 Z-0.0100
G01 X9.8792 Y-4.7371 Z-0.0100
G01 X9.8235 Y-4.7450 Z-0.0100
G01 X9.7705 Y-4.7526 Z-0.0100
G01 X9.7198 Y-4.7600 Z-0.0100
G01 X9.7189 Y-4.7601 Z-0.0100
G01 X9.6713 Y-4.7675 Z-0.0100
G01 X9.6701 Y-4.7676 Z-0.0100
G01 X9.6250 Y-4.7750 Z-0.0100
G01 X9.6232 Y-4.7753 Z-0.0100
G01 X9.5805 Y-4.7829 Z-0.0100
G01 X9.5781 Y-4.7834 Z-0.0100
G01 X9.5379 Y-4.7914 Z-0.0100
G01 X9.5350 Y-4.7920 Z-0.0100
G01 X9.4970 Y-4.8004 Z-0.0100
G01 X9.4934 Y-4.8013 Z-0.0100
G01 X9.4577 Y-4.8104 Z-0.0100
G01 X9.4532 Y-4.8116 Z-0.0100
G01 X9.4197 Y-4.8215 Z-0.0100
G01 X9.4144 Y-4.8232 Z-0.0100
G01 X9.3830 Y-4.8340 Z-0.0100
G01 X9.3769 Y-4.8362 Z-0.0100
G01 X9.3475 Y-4.8481 Z-0.0100
G01 X9.3407 Y-4.8511 Z-0.0100
G01 X9.3133 Y-4.8640 Z-0.0100
G01 X9.3055 Y-4.8681 Z-0.0100
G01 X9.2801 Y-4.8824 Z-0.0100
G01 X9.2718 Y-4.8875 Z-0.0100
G01 X9.2482 Y-4.9033 Z-0.0100
G01 X9.2401 Y-4.9092 Z-0.0100
G01 X9.2182 Y-4.9266 Z-0.0100
G01 X9.2098 Y-4.9338 Z-0.0100
G01 X9.1896 Y-4.9530 Z-0.0100
G01 X9.1820 Y-4.9609 Z-0.0100
G01 X9.1635 Y-4.9819 Z-0.0100
G01 X9.1565 Y-4.9905 Z-0.0100
G01 X9.1396 Y-5.0135 Z-0.0100
G01 X9.1338 Y-5.0222 Z-0.0100
G01 X9.1183 Y-5.0473 Z-0.0100
G01 X9.1135 Y-5.0558 Z-0.0100
G01 X9.0994 Y-5.0833 Z-0.0100
G01 X9.0957 Y-5.0914 Z-0.0100
G01 X9.0830 Y-5.1212 Z-0.0100
G01 X9.0801 Y-5.1287 Z-0.0100
G01 X9.0687 Y-5.1611 Z-0.0100
G01 X9.0666 Y-5.1678 Z-0.0100
G01 X9.0564 Y-5.2029 Z-0.0100
G01 X9.0549 Y-5.2087 Z-0.0100
G01 X9.0459 Y-5.2466 Z-0.0100
G01 X9.0448 Y-5.2517 Z-0.0100
G01 X9.0368 Y-5.2927 Z-0.0100
G01 X9.0360 Y-5.2972 Z-0.0100
G01 X9.0291 Y-5.3412 Z-0.0100
G01 X9.0286 Y-5.3448 Z-0.0100
G01 X9.0226 Y-5.3921 Z-0.0100
G01 X9.0223 Y-5.3953 Z-0.0100
G01 X9.0172 Y-5.4459 Z-0.0100
G01 X9.0170 Y-5.4487 Z-0.0100
G01 X9.0127 Y-5.5029 Z-0.0100
G01 X9.0125 Y-5.5050 Z-0.0100
G01 X9.0090 Y-5.5628 Z-0.0100
G01 X9.0089 Y-5.5646 Z-0.0100
G01 X9.0060 Y-5.6262 Z-0.0100
G01 X9.0060 Y-5.6278 Z-0.0100
G01 X9.0037 Y-5.6934 Z-0.0100
G01 X9.0037 Y-5.6946 Z-0.0100
G01 X9.0020 Y-5.7642 Z-0.0100
G01 X9.0019 Y-5.7651 Z-0.0100
G01 X9.0007 Y-5.8390 Z-0.0100
G01 X9.0007 Y-5.8398 Z-0.0100
G01 X8.9998 Y-5.9179 Z-0.0100
G01 X8.9998 Y-5.9185 Z-0.0100
G01 X8.9993 Y-6.0012 Z-0.0100
G01 X8.9990 Y-6.0889 Z-0.0100
G01 X8.9990 Y-6.1814 Z-0.0100
G00 X8.9990 Y-6.1814 Z0.2500
G00 X8.2116 Y-6.2600 Z0.2500
M05
G00 X0.0000 Y0.0000 Z0.2500
G49
G01 G55 Z0 F20
G00 G55 X0 Y0 Z0
M30
