( Sample-splitboard-base-cutout.nc                                             )
( Generated by MonkeyCAM v4.0.6 master@6e00d93                                 )
(   at 2018-03-07 15:09:35 PST                                                 )
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
( * Cutter bounding box in G54:                                                )
(     X-0.0452 Y-5.7564 Z-0.0100 to X66.1872 Y0.0462 Z0.2500                   )

G90 G20 G17 G40 G49
G43 H1 T1
G00 G54 X0 Y0 Z0.2500

G00 X-0.0452 Y-0.0802 Z0.2500
M03
G04 P5
G01 X-0.0452 Y-0.0802 Z-0.0100 F80
G01 X-0.0441 Y-0.1698 Z-0.0100
G01 X-0.0440 Y-0.1730 Z-0.0100
G01 X-0.0376 Y-0.3441 Z-0.0100
G01 X-0.0374 Y-0.3474 Z-0.0100
G01 X-0.0267 Y-0.5150 Z-0.0100
G01 X-0.0264 Y-0.5183 Z-0.0100
G01 X-0.0111 Y-0.6879 Z-0.0100
G01 X-0.0107 Y-0.6912 Z-0.0100
G01 X0.0088 Y-0.8578 Z-0.0100
G01 X0.0092 Y-0.8612 Z-0.0100
G01 X0.0336 Y-1.0285 Z-0.0100
G01 X0.0341 Y-1.0320 Z-0.0100
G01 X0.0632 Y-1.1984 Z-0.0100
G01 X0.0639 Y-1.2019 Z-0.0100
G01 X0.0975 Y-1.3657 Z-0.0100
G01 X0.0983 Y-1.3693 Z-0.0100
G01 X0.1372 Y-1.5346 Z-0.0100
G01 X0.1381 Y-1.5383 Z-0.0100
G01 X0.1818 Y-1.7012 Z-0.0100
G01 X0.1829 Y-1.7049 Z-0.0100
G01 X0.2314 Y-1.8659 Z-0.0100
G01 X0.2325 Y-1.8697 Z-0.0100
G01 X0.2864 Y-2.0301 Z-0.0100
G01 X0.2878 Y-2.0339 Z-0.0100
G01 X0.3466 Y-2.1918 Z-0.0100
G01 X0.3481 Y-2.1956 Z-0.0100
G01 X0.4119 Y-2.3512 Z-0.0100
G01 X0.4136 Y-2.3551 Z-0.0100
G01 X0.4833 Y-2.5100 Z-0.0100
G01 X0.4851 Y-2.5138 Z-0.0100
G01 X0.5605 Y-2.6671 Z-0.0100
G01 X0.5624 Y-2.6709 Z-0.0100
G01 X0.6422 Y-2.8200 Z-0.0100
G01 X0.6444 Y-2.8238 Z-0.0100
G01 X0.7308 Y-2.9724 Z-0.0100
G01 X0.7331 Y-2.9762 Z-0.0100
G01 X0.8251 Y-3.1220 Z-0.0100
G01 X0.8275 Y-3.1258 Z-0.0100
G01 X0.9248 Y-3.2683 Z-0.0100
G01 X0.9274 Y-3.2720 Z-0.0100
G01 X1.0305 Y-3.4120 Z-0.0100
G01 X1.0334 Y-3.4157 Z-0.0100
G01 X1.1427 Y-3.5531 Z-0.0100
G01 X1.1456 Y-3.5566 Z-0.0100
G01 X1.2605 Y-3.6905 Z-0.0100
G01 X1.2636 Y-3.6940 Z-0.0100
G01 X1.3856 Y-3.8259 Z-0.0100
G01 X1.3888 Y-3.8293 Z-0.0100
G01 X1.5164 Y-3.9574 Z-0.0100
G01 X1.5198 Y-3.9606 Z-0.0100
G01 X1.6538 Y-4.0855 Z-0.0100
G01 X1.6573 Y-4.0886 Z-0.0100
G01 X1.7970 Y-4.2094 Z-0.0100
G01 X1.8006 Y-4.2124 Z-0.0100
G01 X1.9481 Y-4.3306 Z-0.0100
G01 X1.9518 Y-4.3334 Z-0.0100
G01 X2.1050 Y-4.4471 Z-0.0100
G01 X2.1087 Y-4.4498 Z-0.0100
G01 X2.2683 Y-4.5595 Z-0.0100
G01 X2.2721 Y-4.5620 Z-0.0100
G01 X2.4394 Y-4.6684 Z-0.0100
G01 X2.4433 Y-4.6707 Z-0.0100
G01 X2.6162 Y-4.7722 Z-0.0100
G01 X2.6201 Y-4.7743 Z-0.0100
G01 X2.8025 Y-4.8729 Z-0.0100
G01 X2.8063 Y-4.8749 Z-0.0100
G01 X2.9919 Y-4.9670 Z-0.0100
G01 X2.9958 Y-4.9688 Z-0.0100
G01 X3.1922 Y-5.0581 Z-0.0100
G01 X3.1959 Y-5.0598 Z-0.0100
G01 X3.3973 Y-5.1434 Z-0.0100
G01 X3.4011 Y-5.1449 Z-0.0100
G01 X3.6114 Y-5.2244 Z-0.0100
G01 X3.6152 Y-5.2257 Z-0.0100
G01 X3.8305 Y-5.2993 Z-0.0100
G01 X3.8342 Y-5.3005 Z-0.0100
G01 X4.0594 Y-5.3698 Z-0.0100
G01 X4.0630 Y-5.3709 Z-0.0100
G01 X4.2962 Y-5.4349 Z-0.0100
G01 X4.2998 Y-5.4358 Z-0.0100
G01 X4.5379 Y-5.4938 Z-0.0100
G01 X4.5414 Y-5.4946 Z-0.0100
G01 X4.7894 Y-5.5475 Z-0.0100
G01 X4.7929 Y-5.5482 Z-0.0100
G01 X5.0484 Y-5.5953 Z-0.0100
G01 X5.0516 Y-5.5959 Z-0.0100
G01 X5.3156 Y-5.6374 Z-0.0100
G01 X5.3189 Y-5.6378 Z-0.0100
G01 X5.5894 Y-5.6730 Z-0.0100
G01 X5.5926 Y-5.6734 Z-0.0100
G01 X5.8715 Y-5.7026 Z-0.0100
G01 X5.8746 Y-5.7029 Z-0.0100
G01 X6.1635 Y-5.7260 Z-0.0100
G01 X6.1664 Y-5.7262 Z-0.0100
G01 X6.4622 Y-5.7427 Z-0.0100
G01 X6.4651 Y-5.7429 Z-0.0100
G01 X6.7694 Y-5.7529 Z-0.0100
G01 X6.7722 Y-5.7530 Z-0.0100
G01 X7.0833 Y-5.7564 Z-0.0100
G01 X7.0925 Y-5.7561 Z-0.0100
G01 X7.6242 Y-5.7227 Z-0.0100
G01 X8.1588 Y-5.6898 Z-0.0100
G01 X8.6936 Y-5.6575 Z-0.0100
G01 X8.6960 Y-5.6574 Z-0.0100
G01 X9.2272 Y-5.6260 Z-0.0100
G01 X9.2283 Y-5.6260 Z-0.0100
G01 X9.7632 Y-5.5950 Z-0.0100
G01 X10.2981 Y-5.5648 Z-0.0100
G01 X10.8329 Y-5.5351 Z-0.0100
G01 X11.3666 Y-5.5063 Z-0.0100
G01 X11.3679 Y-5.5062 Z-0.0100
G01 X11.9029 Y-5.4780 Z-0.0100
G01 X12.4379 Y-5.4503 Z-0.0100
G01 X12.9729 Y-5.4234 Z-0.0100
G01 X12.9759 Y-5.4232 Z-0.0100
G01 X13.5080 Y-5.3970 Z-0.0100
G01 X14.0417 Y-5.3715 Z-0.0100
G01 X14.0431 Y-5.3714 Z-0.0100
G01 X14.5783 Y-5.3465 Z-0.0100
G01 X15.1134 Y-5.3222 Z-0.0100
G01 X15.6486 Y-5.2985 Z-0.0100
G01 X16.1838 Y-5.2755 Z-0.0100
G01 X16.7191 Y-5.2532 Z-0.0100
G01 X17.2527 Y-5.2316 Z-0.0100
G01 X17.2544 Y-5.2316 Z-0.0100
G01 X17.7897 Y-5.2106 Z-0.0100
G01 X17.7915 Y-5.2105 Z-0.0100
G01 X18.3250 Y-5.1902 Z-0.0100
G01 X18.8604 Y-5.1705 Z-0.0100
G01 X19.3958 Y-5.1515 Z-0.0100
G01 X19.9312 Y-5.1332 Z-0.0100
G01 X20.4666 Y-5.1155 Z-0.0100
G01 X21.0002 Y-5.0985 Z-0.0100
G01 X21.0020 Y-5.0985 Z-0.0100
G01 X21.5375 Y-5.0822 Z-0.0100
G01 X22.0730 Y-5.0664 Z-0.0100
G01 X22.6085 Y-5.0514 Z-0.0100
G01 X23.1440 Y-5.0370 Z-0.0100
G01 X23.6796 Y-5.0233 Z-0.0100
G01 X24.2151 Y-5.0103 Z-0.0100
G01 X24.7507 Y-4.9979 Z-0.0100
G01 X24.7544 Y-4.9978 Z-0.0100
G01 X25.2863 Y-4.9861 Z-0.0100
G01 X25.8219 Y-4.9751 Z-0.0100
G01 X26.3575 Y-4.9647 Z-0.0100
G01 X26.8910 Y-4.9550 Z-0.0100
G01 X26.8931 Y-4.9550 Z-0.0100
G01 X27.4288 Y-4.9459 Z-0.0100
G01 X27.9644 Y-4.9375 Z-0.0100
G01 X28.5001 Y-4.9297 Z-0.0100
G01 X29.0358 Y-4.9227 Z-0.0100
G01 X29.5714 Y-4.9162 Z-0.0100
G01 X30.1072 Y-4.9105 Z-0.0100
G01 X30.6428 Y-4.9054 Z-0.0100
G01 X31.1785 Y-4.9009 Z-0.0100
G01 X31.7142 Y-4.8972 Z-0.0100
G01 X32.2499 Y-4.8940 Z-0.0100
G01 X32.7856 Y-4.8916 Z-0.0100
G01 X33.3213 Y-4.8898 Z-0.0100
G01 X33.8571 Y-4.8887 Z-0.0100
G01 X34.3928 Y-4.8883 Z-0.0100
G01 X34.9285 Y-4.8885 Z-0.0100
G01 X35.4642 Y-4.8893 Z-0.0100
G01 X35.9999 Y-4.8909 Z-0.0100
G01 X36.5356 Y-4.8931 Z-0.0100
G01 X37.0713 Y-4.8959 Z-0.0100
G01 X37.6070 Y-4.8994 Z-0.0100
G01 X38.1427 Y-4.9036 Z-0.0100
G01 X38.6784 Y-4.9085 Z-0.0100
G01 X39.2141 Y-4.9140 Z-0.0100
G01 X39.7498 Y-4.9201 Z-0.0100
G01 X40.2855 Y-4.9270 Z-0.0100
G01 X40.8211 Y-4.9345 Z-0.0100
G01 X41.3568 Y-4.9426 Z-0.0100
G01 X41.8924 Y-4.9514 Z-0.0100
G01 X42.4281 Y-4.9609 Z-0.0100
G01 X42.4301 Y-4.9609 Z-0.0100
G01 X42.9637 Y-4.9711 Z-0.0100
G01 X43.4993 Y-4.9819 Z-0.0100
G01 X44.0312 Y-4.9933 Z-0.0100
G01 X44.0349 Y-4.9934 Z-0.0100
G01 X44.5705 Y-5.0055 Z-0.0100
G01 X45.1060 Y-5.0183 Z-0.0100
G01 X45.6416 Y-5.0318 Z-0.0100
G01 X46.1771 Y-5.0459 Z-0.0100
G01 X46.7127 Y-5.0607 Z-0.0100
G01 X47.2481 Y-5.0761 Z-0.0100
G01 X47.7837 Y-5.0923 Z-0.0100
G01 X47.7855 Y-5.0923 Z-0.0100
G01 X48.3191 Y-5.1090 Z-0.0100
G01 X48.8545 Y-5.1264 Z-0.0100
G01 X49.3899 Y-5.1445 Z-0.0100
G01 X49.9253 Y-5.1633 Z-0.0100
G01 X50.4607 Y-5.1827 Z-0.0100
G01 X50.9942 Y-5.2027 Z-0.0100
G01 X50.9960 Y-5.2028 Z-0.0100
G01 X51.5314 Y-5.2236 Z-0.0100
G01 X51.5329 Y-5.2237 Z-0.0100
G01 X52.0666 Y-5.2450 Z-0.0100
G01 X52.6019 Y-5.2670 Z-0.0100
G01 X53.1371 Y-5.2898 Z-0.0100
G01 X53.6723 Y-5.3132 Z-0.0100
G01 X54.2075 Y-5.3372 Z-0.0100
G01 X54.7427 Y-5.3620 Z-0.0100
G01 X54.7441 Y-5.3620 Z-0.0100
G01 X55.2777 Y-5.3873 Z-0.0100
G01 X55.8129 Y-5.4133 Z-0.0100
G01 X56.3448 Y-5.4399 Z-0.0100
G01 X56.3479 Y-5.4401 Z-0.0100
G01 X56.8829 Y-5.4675 Z-0.0100
G01 X57.4180 Y-5.4955 Z-0.0100
G01 X57.4192 Y-5.4955 Z-0.0100
G01 X57.9529 Y-5.5242 Z-0.0100
G01 X58.4877 Y-5.5535 Z-0.0100
G01 X59.0226 Y-5.5835 Z-0.0100
G01 X59.5575 Y-5.6142 Z-0.0100
G01 X60.0916 Y-5.6455 Z-0.0100
G01 X60.0922 Y-5.6456 Z-0.0100
G01 X60.6238 Y-5.6774 Z-0.0100
G01 X60.6331 Y-5.6776 Z-0.0100
G01 X60.8748 Y-5.6743 Z-0.0100
G01 X60.8782 Y-5.6742 Z-0.0100
G01 X61.1132 Y-5.6644 Z-0.0100
G01 X61.1168 Y-5.6642 Z-0.0100
G01 X61.3463 Y-5.6479 Z-0.0100
G01 X61.3501 Y-5.6475 Z-0.0100
G01 X61.5730 Y-5.6250 Z-0.0100
G01 X61.5769 Y-5.6245 Z-0.0100
G01 X61.7944 Y-5.5956 Z-0.0100
G01 X61.7983 Y-5.5950 Z-0.0100
G01 X62.0069 Y-5.5605 Z-0.0100
G01 X62.0109 Y-5.5598 Z-0.0100
G01 X62.2163 Y-5.5190 Z-0.0100
G01 X62.2204 Y-5.5181 Z-0.0100
G01 X62.4166 Y-5.4722 Z-0.0100
G01 X62.4208 Y-5.4712 Z-0.0100
G01 X62.6134 Y-5.4190 Z-0.0100
G01 X62.6176 Y-5.4178 Z-0.0100
G01 X62.8025 Y-5.3607 Z-0.0100
G01 X62.8067 Y-5.3593 Z-0.0100
G01 X62.9875 Y-5.2963 Z-0.0100
G01 X62.9917 Y-5.2948 Z-0.0100
G01 X63.1637 Y-5.2277 Z-0.0100
G01 X63.1681 Y-5.2259 Z-0.0100
G01 X63.3366 Y-5.1529 Z-0.0100
G01 X63.3409 Y-5.1509 Z-0.0100
G01 X63.5029 Y-5.0733 Z-0.0100
G01 X63.5071 Y-5.0712 Z-0.0100
G01 X63.6637 Y-4.9887 Z-0.0100
G01 X63.6679 Y-4.9864 Z-0.0100
G01 X63.8188 Y-4.8994 Z-0.0100
G01 X63.8230 Y-4.8969 Z-0.0100
G01 X63.9684 Y-4.8053 Z-0.0100
G01 X63.9724 Y-4.8027 Z-0.0100
G01 X64.1121 Y-4.7070 Z-0.0100
G01 X64.1161 Y-4.7042 Z-0.0100
G01 X64.2506 Y-4.6042 Z-0.0100
G01 X64.2544 Y-4.6012 Z-0.0100
G01 X64.3834 Y-4.4972 Z-0.0100
G01 X64.3871 Y-4.4941 Z-0.0100
G01 X64.5116 Y-4.3856 Z-0.0100
G01 X64.5152 Y-4.3824 Z-0.0100
G01 X64.6335 Y-4.2710 Z-0.0100
G01 X64.6369 Y-4.2677 Z-0.0100
G01 X64.7518 Y-4.1509 Z-0.0100
G01 X64.7550 Y-4.1475 Z-0.0100
G01 X64.8629 Y-4.0292 Z-0.0100
G01 X64.8660 Y-4.0257 Z-0.0100
G01 X64.9701 Y-3.9029 Z-0.0100
G01 X64.9730 Y-3.8993 Z-0.0100
G01 X65.0728 Y-3.7722 Z-0.0100
G01 X65.0755 Y-3.7687 Z-0.0100
G01 X65.1688 Y-3.6407 Z-0.0100
G01 X65.1714 Y-3.6370 Z-0.0100
G01 X65.2619 Y-3.5033 Z-0.0100
G01 X65.2642 Y-3.4996 Z-0.0100
G01 X65.3488 Y-3.3648 Z-0.0100
G01 X65.3511 Y-3.3612 Z-0.0100
G01 X65.4316 Y-3.2225 Z-0.0100
G01 X65.4337 Y-3.2190 Z-0.0100
G01 X65.5087 Y-3.0795 Z-0.0100
G01 X65.5105 Y-3.0759 Z-0.0100
G01 X65.5826 Y-2.9310 Z-0.0100
G01 X65.5843 Y-2.9274 Z-0.0100
G01 X65.6517 Y-2.7805 Z-0.0100
G01 X65.6532 Y-2.7771 Z-0.0100
G01 X65.7150 Y-2.6309 Z-0.0100
G01 X65.7163 Y-2.6275 Z-0.0100
G01 X65.7751 Y-2.4758 Z-0.0100
G01 X65.7764 Y-2.4724 Z-0.0100
G01 X65.8305 Y-2.3198 Z-0.0100
G01 X65.8316 Y-2.3165 Z-0.0100
G01 X65.8817 Y-2.1618 Z-0.0100
G01 X65.8827 Y-2.1586 Z-0.0100
G01 X65.9289 Y-2.0014 Z-0.0100
G01 X65.9298 Y-1.9981 Z-0.0100
G01 X65.9711 Y-1.8422 Z-0.0100
G01 X65.9719 Y-1.8390 Z-0.0100
G01 X66.0101 Y-1.6785 Z-0.0100
G01 X66.0108 Y-1.6755 Z-0.0100
G01 X66.0450 Y-1.5136 Z-0.0100
G01 X66.0455 Y-1.5106 Z-0.0100
G01 X66.0757 Y-1.3484 Z-0.0100
G01 X66.0762 Y-1.3455 Z-0.0100
G01 X66.1022 Y-1.1840 Z-0.0100
G01 X66.1027 Y-1.1811 Z-0.0100
G01 X66.1253 Y-1.0168 Z-0.0100
G01 X66.1257 Y-1.0140 Z-0.0100
G01 X66.1449 Y-0.8471 Z-0.0100
G01 X66.1452 Y-0.8444 Z-0.0100
G01 X66.1604 Y-0.6802 Z-0.0100
G01 X66.1607 Y-0.6775 Z-0.0100
G01 X66.1726 Y-0.5094 Z-0.0100
G01 X66.1728 Y-0.5068 Z-0.0100
G01 X66.1811 Y-0.3415 Z-0.0100
G01 X66.1812 Y-0.3390 Z-0.0100
G01 X66.1862 Y-0.1718 Z-0.0100
G01 X66.1862 Y-0.1693 Z-0.0100
G01 X66.1872 Y-0.0800 Z-0.0100
G01 X66.1871 Y-0.0764 Z-0.0100
G01 X66.1870 Y-0.0728 Z-0.0100
G01 X66.1868 Y-0.0692 Z-0.0100
G01 X66.1865 Y-0.0656 Z-0.0100
G01 X66.1861 Y-0.0620 Z-0.0100
G01 X66.1855 Y-0.0585 Z-0.0100
G01 X66.1849 Y-0.0550 Z-0.0100
G01 X66.1842 Y-0.0514 Z-0.0100
G01 X66.1833 Y-0.0479 Z-0.0100
G01 X66.1824 Y-0.0445 Z-0.0100
G01 X66.1813 Y-0.0410 Z-0.0100
G01 X66.1802 Y-0.0376 Z-0.0100
G01 X66.1790 Y-0.0342 Z-0.0100
G01 X66.1777 Y-0.0309 Z-0.0100
G01 X66.1762 Y-0.0276 Z-0.0100
G01 X66.1747 Y-0.0244 Z-0.0100
G01 X66.1731 Y-0.0211 Z-0.0100
G01 X66.1714 Y-0.0180 Z-0.0100
G01 X66.1696 Y-0.0149 Z-0.0100
G01 X66.1677 Y-0.0118 Z-0.0100
G01 X66.1658 Y-0.0088 Z-0.0100
G01 X66.1637 Y-0.0058 Z-0.0100
G01 X66.1616 Y-0.0029 Z-0.0100
G01 X66.1594 Y-0.0001 Z-0.0100
G01 X66.1571 Y0.0025 Z-0.0100
G01 X66.1547 Y0.0052 Z-0.0100
G01 X66.1522 Y0.0079 Z-0.0100
G01 X66.1497 Y0.0104 Z-0.0100
G01 X66.1471 Y0.0129 Z-0.0100
G01 X66.1444 Y0.0153 Z-0.0100
G01 X66.1417 Y0.0176 Z-0.0100
G01 X66.1389 Y0.0199 Z-0.0100
G01 X66.1361 Y0.0220 Z-0.0100
G01 X66.1331 Y0.0241 Z-0.0100
G01 X66.1301 Y0.0261 Z-0.0100
G01 X66.1271 Y0.0280 Z-0.0100
G01 X66.1240 Y0.0298 Z-0.0100
G01 X66.1208 Y0.0316 Z-0.0100
G01 X66.1176 Y0.0332 Z-0.0100
G01 X66.1144 Y0.0348 Z-0.0100
G01 X66.1111 Y0.0362 Z-0.0100
G01 X66.1078 Y0.0376 Z-0.0100
G01 X66.1044 Y0.0388 Z-0.0100
G01 X66.1010 Y0.0400 Z-0.0100
G01 X66.0976 Y0.0411 Z-0.0100
G01 X66.0941 Y0.0420 Z-0.0100
G01 X66.0907 Y0.0429 Z-0.0100
G01 X66.0872 Y0.0437 Z-0.0100
G01 X66.0836 Y0.0444 Z-0.0100
G01 X66.0801 Y0.0449 Z-0.0100
G01 X66.0765 Y0.0454 Z-0.0100
G01 X66.0729 Y0.0457 Z-0.0100
G01 X66.0693 Y0.0460 Z-0.0100
G01 X66.0657 Y0.0462 Z-0.0100
G01 X66.0622 Y0.0462 Z-0.0100
G01 X0.0797 Y0.0462 Z-0.0100
G01 X0.0761 Y0.0462 Z-0.0100
G01 X0.0725 Y0.0460 Z-0.0100
G01 X0.0689 Y0.0457 Z-0.0100
G01 X0.0653 Y0.0454 Z-0.0100
G01 X0.0618 Y0.0449 Z-0.0100
G01 X0.0582 Y0.0444 Z-0.0100
G01 X0.0547 Y0.0437 Z-0.0100
G01 X0.0511 Y0.0429 Z-0.0100
G01 X0.0477 Y0.0420 Z-0.0100
G01 X0.0442 Y0.0411 Z-0.0100
G01 X0.0407 Y0.0400 Z-0.0100
G01 X0.0374 Y0.0388 Z-0.0100
G01 X0.0340 Y0.0375 Z-0.0100
G01 X0.0307 Y0.0362 Z-0.0100
G01 X0.0274 Y0.0348 Z-0.0100
G01 X0.0241 Y0.0332 Z-0.0100
G01 X0.0209 Y0.0315 Z-0.0100
G01 X0.0178 Y0.0298 Z-0.0100
G01 X0.0147 Y0.0280 Z-0.0100
G01 X0.0116 Y0.0261 Z-0.0100
G01 X0.0087 Y0.0240 Z-0.0100
G01 X0.0057 Y0.0220 Z-0.0100
G01 X0.0028 Y0.0198 Z-0.0100
G01 X0.0000 Y0.0175 Z-0.0100
G01 X-0.0026 Y0.0152 Z-0.0100
G01 X-0.0053 Y0.0128 Z-0.0100
G01 X-0.0079 Y0.0103 Z-0.0100
G01 X-0.0104 Y0.0077 Z-0.0100
G01 X-0.0129 Y0.0051 Z-0.0100
G01 X-0.0152 Y0.0024 Z-0.0100
G01 X-0.0175 Y-0.0002 Z-0.0100
G01 X-0.0198 Y-0.0031 Z-0.0100
G01 X-0.0219 Y-0.0060 Z-0.0100
G01 X-0.0239 Y-0.0089 Z-0.0100
G01 X-0.0259 Y-0.0119 Z-0.0100
G01 X-0.0278 Y-0.0150 Z-0.0100
G01 X-0.0296 Y-0.0181 Z-0.0100
G01 X-0.0313 Y-0.0213 Z-0.0100
G01 X-0.0329 Y-0.0245 Z-0.0100
G01 X-0.0344 Y-0.0278 Z-0.0100
G01 X-0.0358 Y-0.0311 Z-0.0100
G01 X-0.0371 Y-0.0344 Z-0.0100
G01 X-0.0383 Y-0.0378 Z-0.0100
G01 X-0.0395 Y-0.0412 Z-0.0100
G01 X-0.0405 Y-0.0447 Z-0.0100
G01 X-0.0414 Y-0.0482 Z-0.0100
G01 X-0.0423 Y-0.0516 Z-0.0100
G01 X-0.0430 Y-0.0552 Z-0.0100
G01 X-0.0436 Y-0.0587 Z-0.0100
G01 X-0.0442 Y-0.0623 Z-0.0100
G01 X-0.0446 Y-0.0659 Z-0.0100
G01 X-0.0449 Y-0.0694 Z-0.0100
G01 X-0.0451 Y-0.0730 Z-0.0100
G01 X-0.0452 Y-0.0766 Z-0.0100
G01 X-0.0452 Y-0.0802 Z-0.0100
G00 X-0.0452 Y-0.0802 Z0.2500
M05
G00 X0.0000 Y0.0000 Z0.2500
G49
G01 G55 Z0 F20
G00 G55 X0 Y0 Z0
M30