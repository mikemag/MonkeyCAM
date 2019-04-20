( Sample-splitboard-core-edge-groove.nc                                        )
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
( * Rapid height: 0.2000" [0.5080cm]                                           )
( * Tool: T1, Quarter Inch Upcut Spiral, diameter 0.2469" [0.6273cm]           )
( * Height baseline [Z=0.0] is the top of the material.                        )
( * XY origin is the left side of the table, Y is center of the part.          )
( * Requires G54 to be the part work coordinate offsets [WCO]. [0, 0] is the   )
(   center of the nose, with the board extending to positive X. The cutter may )
(   move to negative values of X.                                              )
( * Requires G55 to be the machine WCO, with Z above the rapid height.         )
( * Units are inches                                                           )
( * Center of the board in G54:                                                )
(     X=33.8582 Y=0.0000 Z=0.0000                                              )
( * Cutter bounding box in G54:                                                )
(     X0.0000 Y-5.7547 Z-0.0300 to X66.1362 Y5.7547 Z0.2000                    )

G90 G20 G17 G40 G49
G43 H1 T1
G00 G54 X0 Y0 Z0.2000

M03
G04 P5
G00 X0.2305 Y-0.0471 Z0.2000
G01 X0.2305 Y-0.0471 Z-0.0300 F80
G01 X0.2305 Y-0.0471 Z-0.0300
G01 X0.2325 Y-0.2159 Z-0.0300
G01 X0.2388 Y-0.3811 Z-0.0300
G01 X0.2493 Y-0.5455 Z-0.0300
G01 X0.2641 Y-0.7095 Z-0.0300
G01 X0.2834 Y-0.8735 Z-0.0300
G01 X0.3071 Y-1.0360 Z-0.0300
G01 X0.3355 Y-1.1987 Z-0.0300
G01 X0.3684 Y-1.3591 Z-0.0300
G01 X0.4062 Y-1.5193 Z-0.0300
G01 X0.4484 Y-1.6768 Z-0.0300
G01 X0.4957 Y-1.8338 Z-0.0300
G01 X0.5480 Y-1.9896 Z-0.0300
G01 X0.6049 Y-2.1424 Z-0.0300
G01 X0.6675 Y-2.2951 Z-0.0300
G01 X0.7344 Y-2.4436 Z-0.0300
G01 X0.8072 Y-2.5916 Z-0.0300
G01 X0.8855 Y-2.7379 Z-0.0300
G01 X0.9688 Y-2.8812 Z-0.0300
G01 X1.0573 Y-3.0215 Z-0.0300
G01 X1.1514 Y-3.1594 Z-0.0300
G01 X1.2519 Y-3.2959 Z-0.0300
G01 X1.3576 Y-3.4287 Z-0.0300
G01 X1.4690 Y-3.5585 Z-0.0300
G01 X1.5864 Y-3.6855 Z-0.0300
G01 X1.7109 Y-3.8104 Z-0.0300
G01 X1.8398 Y-3.9305 Z-0.0300
G01 X1.9762 Y-4.0484 Z-0.0300
G01 X2.1191 Y-4.1629 Z-0.0300
G01 X2.2674 Y-4.2730 Z-0.0300
G01 X2.4231 Y-4.3801 Z-0.0300
G01 X2.5859 Y-4.4835 Z-0.0300
G01 X2.7543 Y-4.5823 Z-0.0300
G01 X2.9303 Y-4.6774 Z-0.0300
G01 X3.1133 Y-4.7682 Z-0.0300
G01 X3.3043 Y-4.8551 Z-0.0300
G01 X3.5020 Y-4.9372 Z-0.0300
G01 X3.7059 Y-5.0142 Z-0.0300
G01 X3.9189 Y-5.0870 Z-0.0300
G01 X4.1381 Y-5.1544 Z-0.0300
G01 X4.3664 Y-5.2172 Z-0.0300
G01 X4.6022 Y-5.2746 Z-0.0300
G01 X4.8450 Y-5.3264 Z-0.0300
G01 X5.0970 Y-5.3729 Z-0.0300
G01 X5.3557 Y-5.4135 Z-0.0300
G01 X5.6239 Y-5.4484 Z-0.0300
G01 X5.8995 Y-5.4772 Z-0.0300
G01 X6.1824 Y-5.4998 Z-0.0300
G01 X6.4759 Y-5.5163 Z-0.0300
G01 X6.7766 Y-5.5262 Z-0.0300
G01 X7.0810 Y-5.5296 Z-0.0300
G01 X7.6068 Y-5.4965 Z-0.0300
G01 X8.1418 Y-5.4636 Z-0.0300
G01 X8.6769 Y-5.4313 Z-0.0300
G01 X9.2121 Y-5.3997 Z-0.0300
G01 X9.7472 Y-5.3687 Z-0.0300
G01 X10.2824 Y-5.3384 Z-0.0300
G01 X10.8177 Y-5.3088 Z-0.0300
G01 X11.3530 Y-5.2799 Z-0.0300
G01 X11.8883 Y-5.2516 Z-0.0300
G01 X12.4236 Y-5.2239 Z-0.0300
G01 X12.9590 Y-5.1969 Z-0.0300
G01 X13.4944 Y-5.1706 Z-0.0300
G01 X14.0299 Y-5.1450 Z-0.0300
G01 X14.5653 Y-5.1200 Z-0.0300
G01 X15.1009 Y-5.0957 Z-0.0300
G01 X15.6364 Y-5.0720 Z-0.0300
G01 X16.1720 Y-5.0490 Z-0.0300
G01 X16.7075 Y-5.0267 Z-0.0300
G01 X17.2432 Y-5.0050 Z-0.0300
G01 X17.7788 Y-4.9840 Z-0.0300
G01 X18.3145 Y-4.9637 Z-0.0300
G01 X18.8502 Y-4.9440 Z-0.0300
G01 X19.3859 Y-4.9250 Z-0.0300
G01 X19.9216 Y-4.9066 Z-0.0300
G01 X20.4575 Y-4.8889 Z-0.0300
G01 X20.9932 Y-4.8718 Z-0.0300
G01 X21.5290 Y-4.8555 Z-0.0300
G01 X22.0649 Y-4.8398 Z-0.0300
G01 X22.6007 Y-4.8247 Z-0.0300
G01 X23.1366 Y-4.8103 Z-0.0300
G01 X23.6725 Y-4.7966 Z-0.0300
G01 X24.2084 Y-4.7836 Z-0.0300
G01 X24.7443 Y-4.7712 Z-0.0300
G01 X25.2802 Y-4.7594 Z-0.0300
G01 X25.8162 Y-4.7484 Z-0.0300
G01 X26.3521 Y-4.7380 Z-0.0300
G01 X26.8881 Y-4.7282 Z-0.0300
G01 X27.4241 Y-4.7192 Z-0.0300
G01 X27.9601 Y-4.7107 Z-0.0300
G01 X28.4961 Y-4.7030 Z-0.0300
G01 X29.0321 Y-4.6959 Z-0.0300
G01 X29.5681 Y-4.6894 Z-0.0300
G01 X30.1042 Y-4.6837 Z-0.0300
G01 X30.6402 Y-4.6786 Z-0.0300
G01 X31.1762 Y-4.6742 Z-0.0300
G01 X31.7123 Y-4.6704 Z-0.0300
G01 X32.2483 Y-4.6673 Z-0.0300
G01 X32.7844 Y-4.6648 Z-0.0300
G01 X33.3205 Y-4.6631 Z-0.0300
G01 X33.8565 Y-4.6620 Z-0.0300
G01 X34.3925 Y-4.6615 Z-0.0300
G01 X34.9286 Y-4.6617 Z-0.0300
G01 X35.4647 Y-4.6625 Z-0.0300
G01 X36.0007 Y-4.6641 Z-0.0300
G01 X36.5368 Y-4.6663 Z-0.0300
G01 X37.0728 Y-4.6692 Z-0.0300
G01 X37.6089 Y-4.6727 Z-0.0300
G01 X38.1450 Y-4.6768 Z-0.0300
G01 X38.6810 Y-4.6817 Z-0.0300
G01 X39.2170 Y-4.6872 Z-0.0300
G01 X39.7531 Y-4.6934 Z-0.0300
G01 X40.2890 Y-4.7002 Z-0.0300
G01 X40.8251 Y-4.7077 Z-0.0300
G01 X41.3611 Y-4.7159 Z-0.0300
G01 X41.8970 Y-4.7247 Z-0.0300
G01 X42.4330 Y-4.7342 Z-0.0300
G01 X42.9690 Y-4.7444 Z-0.0300
G01 X43.5050 Y-4.7552 Z-0.0300
G01 X44.0409 Y-4.7666 Z-0.0300
G01 X44.5768 Y-4.7788 Z-0.0300
G01 X45.1127 Y-4.7916 Z-0.0300
G01 X45.6486 Y-4.8051 Z-0.0300
G01 X46.1844 Y-4.8192 Z-0.0300
G01 X46.7203 Y-4.8340 Z-0.0300
G01 X47.2562 Y-4.8494 Z-0.0300
G01 X47.7920 Y-4.8656 Z-0.0300
G01 X48.3278 Y-4.8824 Z-0.0300
G01 X48.8636 Y-4.8998 Z-0.0300
G01 X49.3993 Y-4.9179 Z-0.0300
G01 X49.9350 Y-4.9367 Z-0.0300
G01 X50.4707 Y-4.9561 Z-0.0300
G01 X51.0064 Y-4.9762 Z-0.0300
G01 X51.5421 Y-4.9970 Z-0.0300
G01 X52.0777 Y-5.0184 Z-0.0300
G01 X52.6133 Y-5.0405 Z-0.0300
G01 X53.1489 Y-5.0633 Z-0.0300
G01 X53.6845 Y-5.0867 Z-0.0300
G01 X54.2200 Y-5.1107 Z-0.0300
G01 X54.7555 Y-5.1355 Z-0.0300
G01 X55.2909 Y-5.1609 Z-0.0300
G01 X55.8263 Y-5.1869 Z-0.0300
G01 X56.3618 Y-5.2137 Z-0.0300
G01 X56.8971 Y-5.2411 Z-0.0300
G01 X57.4324 Y-5.2691 Z-0.0300
G01 X57.9677 Y-5.2978 Z-0.0300
G01 X58.5030 Y-5.3272 Z-0.0300
G01 X59.0382 Y-5.3572 Z-0.0300
G01 X59.5734 Y-5.3879 Z-0.0300
G01 X60.1085 Y-5.4193 Z-0.0300
G01 X60.6357 Y-5.4508 Z-0.0300
G01 X60.8674 Y-5.4476 Z-0.0300
G01 X61.1003 Y-5.4379 Z-0.0300
G01 X61.3246 Y-5.4219 Z-0.0300
G01 X61.5429 Y-5.3998 Z-0.0300
G01 X61.7543 Y-5.3718 Z-0.0300
G01 X61.9598 Y-5.3378 Z-0.0300
G01 X62.1596 Y-5.2981 Z-0.0300
G01 X62.3515 Y-5.2532 Z-0.0300
G01 X62.5389 Y-5.2025 Z-0.0300
G01 X62.7179 Y-5.1472 Z-0.0300
G01 X62.8931 Y-5.0861 Z-0.0300
G01 X63.0612 Y-5.0206 Z-0.0300
G01 X63.2238 Y-4.9501 Z-0.0300
G01 X63.3812 Y-4.8747 Z-0.0300
G01 X63.5320 Y-4.7953 Z-0.0300
G01 X63.6779 Y-4.7112 Z-0.0300
G01 X63.8183 Y-4.6228 Z-0.0300
G01 X63.9530 Y-4.5305 Z-0.0300
G01 X64.0830 Y-4.4339 Z-0.0300
G01 X64.2077 Y-4.3333 Z-0.0300
G01 X64.3278 Y-4.2287 Z-0.0300
G01 X64.4418 Y-4.1213 Z-0.0300
G01 X64.5523 Y-4.0091 Z-0.0300
G01 X64.6566 Y-3.8947 Z-0.0300
G01 X64.7575 Y-3.7757 Z-0.0300
G01 X64.8527 Y-3.6544 Z-0.0300
G01 X64.9440 Y-3.5292 Z-0.0300
G01 X65.0312 Y-3.4003 Z-0.0300
G01 X65.1133 Y-3.2695 Z-0.0300
G01 X65.1907 Y-3.1364 Z-0.0300
G01 X65.2643 Y-2.9995 Z-0.0300
G01 X65.3335 Y-2.8603 Z-0.0300
G01 X65.3985 Y-2.7186 Z-0.0300
G01 X65.4596 Y-2.5740 Z-0.0300
G01 X65.5165 Y-2.4274 Z-0.0300
G01 X65.5688 Y-2.2799 Z-0.0300
G01 X65.6179 Y-2.1279 Z-0.0300
G01 X65.6622 Y-1.9774 Z-0.0300
G01 X65.7036 Y-1.8211 Z-0.0300
G01 X65.7402 Y-1.6671 Z-0.0300
G01 X65.7737 Y-1.5085 Z-0.0300
G01 X65.8031 Y-1.3505 Z-0.0300
G01 X65.8288 Y-1.1908 Z-0.0300
G01 X65.8509 Y-1.0307 Z-0.0300
G01 X65.8698 Y-0.8673 Z-0.0300
G01 X65.8848 Y-0.7050 Z-0.0300
G01 X65.8963 Y-0.5433 Z-0.0300
G01 X65.9047 Y-0.3761 Z-0.0300
G01 X65.9095 Y-0.2165 Z-0.0300
G01 X65.9112 Y-0.0476 Z-0.0300
G01 X65.9112 Y0.0476 Z-0.0300
G01 X65.9095 Y0.2156 Z-0.0300
G01 X65.9047 Y0.3761 Z-0.0300
G01 X65.8964 Y0.5424 Z-0.0300
G01 X65.8848 Y0.7050 Z-0.0300
G01 X65.8698 Y0.8673 Z-0.0300
G01 X65.8509 Y1.0307 Z-0.0300
G01 X65.8288 Y1.1908 Z-0.0300
G01 X65.8031 Y1.3505 Z-0.0300
G01 X65.7737 Y1.5085 Z-0.0300
G01 X65.7402 Y1.6671 Z-0.0300
G01 X65.7036 Y1.8211 Z-0.0300
G01 X65.6622 Y1.9774 Z-0.0300
G01 X65.6179 Y2.1279 Z-0.0300
G01 X65.5688 Y2.2799 Z-0.0300
G01 X65.5165 Y2.4274 Z-0.0300
G01 X65.4596 Y2.5740 Z-0.0300
G01 X65.3985 Y2.7186 Z-0.0300
G01 X65.3335 Y2.8603 Z-0.0300
G01 X65.2643 Y2.9995 Z-0.0300
G01 X65.1907 Y3.1364 Z-0.0300
G01 X65.1133 Y3.2695 Z-0.0300
G01 X65.0312 Y3.4003 Z-0.0300
G01 X64.9440 Y3.5292 Z-0.0300
G01 X64.8527 Y3.6544 Z-0.0300
G01 X64.7575 Y3.7757 Z-0.0300
G01 X64.6566 Y3.8947 Z-0.0300
G01 X64.5523 Y4.0091 Z-0.0300
G01 X64.4418 Y4.1213 Z-0.0300
G01 X64.3278 Y4.2287 Z-0.0300
G01 X64.2077 Y4.3333 Z-0.0300
G01 X64.0830 Y4.4339 Z-0.0300
G01 X63.9530 Y4.5305 Z-0.0300
G01 X63.8183 Y4.6228 Z-0.0300
G01 X63.6779 Y4.7112 Z-0.0300
G01 X63.5320 Y4.7953 Z-0.0300
G01 X63.3812 Y4.8747 Z-0.0300
G01 X63.2238 Y4.9501 Z-0.0300
G01 X63.0612 Y5.0206 Z-0.0300
G01 X62.8931 Y5.0861 Z-0.0300
G01 X62.7179 Y5.1472 Z-0.0300
G01 X62.5389 Y5.2025 Z-0.0300
G01 X62.3515 Y5.2532 Z-0.0300
G01 X62.1596 Y5.2981 Z-0.0300
G01 X61.9598 Y5.3378 Z-0.0300
G01 X61.7543 Y5.3718 Z-0.0300
G01 X61.5429 Y5.3998 Z-0.0300
G01 X61.3246 Y5.4219 Z-0.0300
G01 X61.1003 Y5.4379 Z-0.0300
G01 X60.8674 Y5.4476 Z-0.0300
G01 X60.6357 Y5.4508 Z-0.0300
G01 X60.1088 Y5.4193 Z-0.0300
G01 X59.5737 Y5.3879 Z-0.0300
G01 X59.0385 Y5.3572 Z-0.0300
G01 X58.5033 Y5.3272 Z-0.0300
G01 X57.9680 Y5.2978 Z-0.0300
G01 X57.4327 Y5.2691 Z-0.0300
G01 X56.8974 Y5.2411 Z-0.0300
G01 X56.3620 Y5.2137 Z-0.0300
G01 X55.8266 Y5.1869 Z-0.0300
G01 X55.2912 Y5.1609 Z-0.0300
G01 X54.7558 Y5.1355 Z-0.0300
G01 X54.2203 Y5.1107 Z-0.0300
G01 X53.6848 Y5.0867 Z-0.0300
G01 X53.1492 Y5.0633 Z-0.0300
G01 X52.6137 Y5.0405 Z-0.0300
G01 X52.0780 Y5.0184 Z-0.0300
G01 X51.5424 Y4.9970 Z-0.0300
G01 X51.0067 Y4.9762 Z-0.0300
G01 X50.4710 Y4.9561 Z-0.0300
G01 X49.9353 Y4.9367 Z-0.0300
G01 X49.3996 Y4.9179 Z-0.0300
G01 X48.8638 Y4.8998 Z-0.0300
G01 X48.3281 Y4.8824 Z-0.0300
G01 X47.7923 Y4.8656 Z-0.0300
G01 X47.2564 Y4.8495 Z-0.0300
G01 X46.7206 Y4.8340 Z-0.0300
G01 X46.1848 Y4.8192 Z-0.0300
G01 X45.6488 Y4.8051 Z-0.0300
G01 X45.1130 Y4.7916 Z-0.0300
G01 X44.5771 Y4.7788 Z-0.0300
G01 X44.0412 Y4.7666 Z-0.0300
G01 X43.5052 Y4.7552 Z-0.0300
G01 X42.9693 Y4.7444 Z-0.0300
G01 X42.4333 Y4.7342 Z-0.0300
G01 X41.8974 Y4.7247 Z-0.0300
G01 X41.3613 Y4.7159 Z-0.0300
G01 X40.8253 Y4.7077 Z-0.0300
G01 X40.2893 Y4.7003 Z-0.0300
G01 X39.7533 Y4.6934 Z-0.0300
G01 X39.2173 Y4.6872 Z-0.0300
G01 X38.6812 Y4.6817 Z-0.0300
G01 X38.1452 Y4.6768 Z-0.0300
G01 X37.6092 Y4.6727 Z-0.0300
G01 X37.0731 Y4.6692 Z-0.0300
G01 X36.5371 Y4.6663 Z-0.0300
G01 X36.0010 Y4.6641 Z-0.0300
G01 X35.4650 Y4.6625 Z-0.0300
G01 X34.9289 Y4.6617 Z-0.0300
G01 X34.3928 Y4.6615 Z-0.0300
G01 X33.8568 Y4.6620 Z-0.0300
G01 X33.3207 Y4.6631 Z-0.0300
G01 X32.7847 Y4.6648 Z-0.0300
G01 X32.2486 Y4.6673 Z-0.0300
G01 X31.7125 Y4.6704 Z-0.0300
G01 X31.1765 Y4.6742 Z-0.0300
G01 X30.6405 Y4.6786 Z-0.0300
G01 X30.1044 Y4.6837 Z-0.0300
G01 X29.5684 Y4.6894 Z-0.0300
G01 X29.0324 Y4.6959 Z-0.0300
G01 X28.4964 Y4.7030 Z-0.0300
G01 X27.9603 Y4.7107 Z-0.0300
G01 X27.4244 Y4.7192 Z-0.0300
G01 X26.8884 Y4.7282 Z-0.0300
G01 X26.3524 Y4.7380 Z-0.0300
G01 X25.8164 Y4.7484 Z-0.0300
G01 X25.2805 Y4.7594 Z-0.0300
G01 X24.7446 Y4.7712 Z-0.0300
G01 X24.2087 Y4.7836 Z-0.0300
G01 X23.6727 Y4.7966 Z-0.0300
G01 X23.1369 Y4.8103 Z-0.0300
G01 X22.6010 Y4.8247 Z-0.0300
G01 X22.0652 Y4.8398 Z-0.0300
G01 X21.5293 Y4.8555 Z-0.0300
G01 X20.9935 Y4.8718 Z-0.0300
G01 X20.4577 Y4.8889 Z-0.0300
G01 X19.9220 Y4.9066 Z-0.0300
G01 X19.3862 Y4.9249 Z-0.0300
G01 X18.8505 Y4.9440 Z-0.0300
G01 X18.3148 Y4.9637 Z-0.0300
G01 X17.7791 Y4.9840 Z-0.0300
G01 X17.2435 Y5.0050 Z-0.0300
G01 X16.7078 Y5.0267 Z-0.0300
G01 X16.1722 Y5.0490 Z-0.0300
G01 X15.6367 Y5.0720 Z-0.0300
G01 X15.1011 Y5.0957 Z-0.0300
G01 X14.5657 Y5.1200 Z-0.0300
G01 X14.0301 Y5.1450 Z-0.0300
G01 X13.4947 Y5.1706 Z-0.0300
G01 X12.9593 Y5.1969 Z-0.0300
G01 X12.4239 Y5.2239 Z-0.0300
G01 X11.8886 Y5.2516 Z-0.0300
G01 X11.3533 Y5.2798 Z-0.0300
G01 X10.8180 Y5.3088 Z-0.0300
G01 X10.2827 Y5.3384 Z-0.0300
G01 X9.7475 Y5.3687 Z-0.0300
G01 X9.2124 Y5.3997 Z-0.0300
G01 X8.6772 Y5.4312 Z-0.0300
G01 X8.1421 Y5.4636 Z-0.0300
G01 X7.6071 Y5.4965 Z-0.0300
G01 X7.0810 Y5.5296 Z-0.0300
G01 X6.7766 Y5.5262 Z-0.0300
G01 X6.4759 Y5.5163 Z-0.0300
G01 X6.1824 Y5.4998 Z-0.0300
G01 X5.8995 Y5.4772 Z-0.0300
G01 X5.6239 Y5.4484 Z-0.0300
G01 X5.3557 Y5.4135 Z-0.0300
G01 X5.0970 Y5.3729 Z-0.0300
G01 X4.8450 Y5.3264 Z-0.0300
G01 X4.6022 Y5.2746 Z-0.0300
G01 X4.3664 Y5.2172 Z-0.0300
G01 X4.1381 Y5.1544 Z-0.0300
G01 X3.9189 Y5.0870 Z-0.0300
G01 X3.7059 Y5.0142 Z-0.0300
G01 X3.5020 Y4.9372 Z-0.0300
G01 X3.3043 Y4.8551 Z-0.0300
G01 X3.1133 Y4.7682 Z-0.0300
G01 X2.9303 Y4.6774 Z-0.0300
G01 X2.7543 Y4.5823 Z-0.0300
G01 X2.5859 Y4.4835 Z-0.0300
G01 X2.4231 Y4.3801 Z-0.0300
G01 X2.2674 Y4.2730 Z-0.0300
G01 X2.1191 Y4.1629 Z-0.0300
G01 X1.9762 Y4.0484 Z-0.0300
G01 X1.8398 Y3.9305 Z-0.0300
G01 X1.7109 Y3.8104 Z-0.0300
G01 X1.5864 Y3.6855 Z-0.0300
G01 X1.4690 Y3.5585 Z-0.0300
G01 X1.3576 Y3.4287 Z-0.0300
G01 X1.2519 Y3.2959 Z-0.0300
G01 X1.1514 Y3.1594 Z-0.0300
G01 X1.0573 Y3.0215 Z-0.0300
G01 X0.9688 Y2.8812 Z-0.0300
G01 X0.8855 Y2.7379 Z-0.0300
G01 X0.8072 Y2.5916 Z-0.0300
G01 X0.7344 Y2.4436 Z-0.0300
G01 X0.6675 Y2.2951 Z-0.0300
G01 X0.6049 Y2.1424 Z-0.0300
G01 X0.5480 Y1.9896 Z-0.0300
G01 X0.4957 Y1.8338 Z-0.0300
G01 X0.4484 Y1.6768 Z-0.0300
G01 X0.4062 Y1.5193 Z-0.0300
G01 X0.3684 Y1.3591 Z-0.0300
G01 X0.3355 Y1.1987 Z-0.0300
G01 X0.3071 Y1.0360 Z-0.0300
G01 X0.2834 Y0.8735 Z-0.0300
G01 X0.2641 Y0.7095 Z-0.0300
G01 X0.2493 Y0.5455 Z-0.0300
G01 X0.2388 Y0.3807 Z-0.0300
G01 X0.2325 Y0.2159 Z-0.0300
G01 X0.2305 Y0.0471 Z-0.0300
G01 X0.2305 Y-0.0471 Z-0.0300
G01 X0.0054 Y-0.0499 Z-0.0300
G01 X0.0054 Y-0.0499 Z-0.0300
G01 X0.0075 Y-0.2210 Z-0.0300
G01 X0.0140 Y-0.3920 Z-0.0300
G01 X0.0249 Y-0.5629 Z-0.0300
G01 X0.0403 Y-0.7331 Z-0.0300
G01 X0.0603 Y-0.9028 Z-0.0300
G01 X0.0849 Y-1.0719 Z-0.0300
G01 X0.1143 Y-1.2401 Z-0.0300
G01 X0.1486 Y-1.4074 Z-0.0300
G01 X0.1877 Y-1.5736 Z-0.0300
G01 X0.2320 Y-1.7386 Z-0.0300
G01 X0.2812 Y-1.9022 Z-0.0300
G01 X0.3358 Y-2.0644 Z-0.0300
G01 X0.3954 Y-2.2246 Z-0.0300
G01 X0.4605 Y-2.3833 Z-0.0300
G01 X0.5310 Y-2.5400 Z-0.0300
G01 X0.6071 Y-2.6947 Z-0.0300
G01 X0.6888 Y-2.8473 Z-0.0300
G01 X0.7761 Y-2.9973 Z-0.0300
G01 X0.8692 Y-3.1450 Z-0.0300
G01 X0.9681 Y-3.2901 Z-0.0300
G01 X1.0730 Y-3.4324 Z-0.0300
G01 X1.1841 Y-3.5720 Z-0.0300
G01 X1.3011 Y-3.7083 Z-0.0300
G01 X1.4243 Y-3.8416 Z-0.0300
G01 X1.5539 Y-3.9717 Z-0.0300
G01 X1.6900 Y-4.0984 Z-0.0300
G01 X1.8323 Y-4.2215 Z-0.0300
G01 X1.9811 Y-4.3407 Z-0.0300
G01 X2.1367 Y-4.4562 Z-0.0300
G01 X2.2991 Y-4.5679 Z-0.0300
G01 X2.4680 Y-4.6752 Z-0.0300
G01 X2.6441 Y-4.7785 Z-0.0300
G01 X2.8269 Y-4.8773 Z-0.0300
G01 X3.0169 Y-4.9716 Z-0.0300
G01 X3.2142 Y-5.0613 Z-0.0300
G01 X3.4184 Y-5.1462 Z-0.0300
G01 X3.6301 Y-5.2261 Z-0.0300
G01 X3.8493 Y-5.3011 Z-0.0300
G01 X4.0759 Y-5.3707 Z-0.0300
G01 X4.3099 Y-5.4350 Z-0.0300
G01 X4.5517 Y-5.4939 Z-0.0300
G01 X4.8012 Y-5.5472 Z-0.0300
G01 X5.0587 Y-5.5947 Z-0.0300
G01 X5.3239 Y-5.6363 Z-0.0300
G01 X5.5973 Y-5.6719 Z-0.0300
G01 X5.8785 Y-5.7013 Z-0.0300
G01 X6.1682 Y-5.7244 Z-0.0300
G01 X6.4660 Y-5.7411 Z-0.0300
G01 X6.7721 Y-5.7512 Z-0.0300
G01 X7.0863 Y-5.7547 Z-0.0300
G01 X7.6209 Y-5.7211 Z-0.0300
G01 X8.1557 Y-5.6882 Z-0.0300
G01 X8.6905 Y-5.6559 Z-0.0300
G01 X9.2253 Y-5.6243 Z-0.0300
G01 X9.7602 Y-5.5934 Z-0.0300
G01 X10.2951 Y-5.5631 Z-0.0300
G01 X10.8301 Y-5.5335 Z-0.0300
G01 X11.3651 Y-5.5046 Z-0.0300
G01 X11.9002 Y-5.4763 Z-0.0300
G01 X12.4352 Y-5.4487 Z-0.0300
G01 X12.9703 Y-5.4217 Z-0.0300
G01 X13.5055 Y-5.3954 Z-0.0300
G01 X14.0406 Y-5.3698 Z-0.0300
G01 X14.5759 Y-5.3448 Z-0.0300
G01 X15.1111 Y-5.3205 Z-0.0300
G01 X15.6463 Y-5.2968 Z-0.0300
G01 X16.1816 Y-5.2738 Z-0.0300
G01 X16.7169 Y-5.2515 Z-0.0300
G01 X17.2523 Y-5.2299 Z-0.0300
G01 X17.7877 Y-5.2088 Z-0.0300
G01 X18.3231 Y-5.1885 Z-0.0300
G01 X18.8585 Y-5.1688 Z-0.0300
G01 X19.3939 Y-5.1498 Z-0.0300
G01 X19.9294 Y-5.1315 Z-0.0300
G01 X20.4649 Y-5.1138 Z-0.0300
G01 X21.0004 Y-5.0968 Z-0.0300
G01 X21.5359 Y-5.0804 Z-0.0300
G01 X22.0715 Y-5.0647 Z-0.0300
G01 X22.6070 Y-5.0497 Z-0.0300
G01 X23.1426 Y-5.0353 Z-0.0300
G01 X23.6782 Y-5.0216 Z-0.0300
G01 X24.2139 Y-5.0085 Z-0.0300
G01 X24.7495 Y-4.9962 Z-0.0300
G01 X25.2851 Y-4.9844 Z-0.0300
G01 X25.8208 Y-4.9734 Z-0.0300
G01 X26.3565 Y-4.9630 Z-0.0300
G01 X26.8922 Y-4.9532 Z-0.0300
G01 X27.4279 Y-4.9442 Z-0.0300
G01 X27.9636 Y-4.9357 Z-0.0300
G01 X28.4993 Y-4.9280 Z-0.0300
G01 X29.0351 Y-4.9209 Z-0.0300
G01 X29.5708 Y-4.9145 Z-0.0300
G01 X30.1066 Y-4.9087 Z-0.0300
G01 X30.6424 Y-4.9037 Z-0.0300
G01 X31.1781 Y-4.8992 Z-0.0300
G01 X31.7138 Y-4.8954 Z-0.0300
G01 X32.2496 Y-4.8923 Z-0.0300
G01 X32.7854 Y-4.8899 Z-0.0300
G01 X33.3212 Y-4.8881 Z-0.0300
G01 X33.8570 Y-4.8870 Z-0.0300
G01 X34.3927 Y-4.8865 Z-0.0300
G01 X34.9285 Y-4.8867 Z-0.0300
G01 X35.4643 Y-4.8876 Z-0.0300
G01 X36.0001 Y-4.8891 Z-0.0300
G01 X36.5359 Y-4.8913 Z-0.0300
G01 X37.0716 Y-4.8942 Z-0.0300
G01 X37.6074 Y-4.8977 Z-0.0300
G01 X38.1432 Y-4.9019 Z-0.0300
G01 X38.6789 Y-4.9067 Z-0.0300
G01 X39.2147 Y-4.9122 Z-0.0300
G01 X39.7505 Y-4.9184 Z-0.0300
G01 X40.2862 Y-4.9253 Z-0.0300
G01 X40.8219 Y-4.9327 Z-0.0300
G01 X41.3576 Y-4.9409 Z-0.0300
G01 X41.8933 Y-4.9497 Z-0.0300
G01 X42.4290 Y-4.9592 Z-0.0300
G01 X42.9648 Y-4.9694 Z-0.0300
G01 X43.5004 Y-4.9802 Z-0.0300
G01 X44.0361 Y-4.9916 Z-0.0300
G01 X44.5717 Y-5.0038 Z-0.0300
G01 X45.1073 Y-5.0166 Z-0.0300
G01 X45.6429 Y-5.0301 Z-0.0300
G01 X46.1785 Y-5.0442 Z-0.0300
G01 X46.7141 Y-5.0590 Z-0.0300
G01 X47.2497 Y-5.0744 Z-0.0300
G01 X47.7852 Y-5.0905 Z-0.0300
G01 X48.3207 Y-5.1073 Z-0.0300
G01 X48.8562 Y-5.1248 Z-0.0300
G01 X49.3917 Y-5.1428 Z-0.0300
G01 X49.9272 Y-5.1616 Z-0.0300
G01 X50.4626 Y-5.1810 Z-0.0300
G01 X50.9980 Y-5.2011 Z-0.0300
G01 X51.5334 Y-5.2219 Z-0.0300
G01 X52.0687 Y-5.2433 Z-0.0300
G01 X52.6041 Y-5.2653 Z-0.0300
G01 X53.1394 Y-5.2881 Z-0.0300
G01 X53.6746 Y-5.3115 Z-0.0300
G01 X54.2099 Y-5.3355 Z-0.0300
G01 X54.7451 Y-5.3603 Z-0.0300
G01 X55.2803 Y-5.3857 Z-0.0300
G01 X55.8154 Y-5.4117 Z-0.0300
G01 X56.3505 Y-5.4384 Z-0.0300
G01 X56.8856 Y-5.4658 Z-0.0300
G01 X57.4207 Y-5.4938 Z-0.0300
G01 X57.9557 Y-5.5225 Z-0.0300
G01 X58.4907 Y-5.5519 Z-0.0300
G01 X59.0256 Y-5.5819 Z-0.0300
G01 X59.5605 Y-5.6125 Z-0.0300
G01 X60.0953 Y-5.6439 Z-0.0300
G01 X60.6301 Y-5.6759 Z-0.0300
G01 X60.8744 Y-5.6725 Z-0.0300
G01 X61.1124 Y-5.6626 Z-0.0300
G01 X61.3441 Y-5.6461 Z-0.0300
G01 X61.5690 Y-5.6233 Z-0.0300
G01 X61.7880 Y-5.5943 Z-0.0300
G01 X62.0005 Y-5.5592 Z-0.0300
G01 X62.2066 Y-5.5182 Z-0.0300
G01 X62.4067 Y-5.4714 Z-0.0300
G01 X62.6007 Y-5.4189 Z-0.0300
G01 X62.7889 Y-5.3608 Z-0.0300
G01 X62.9707 Y-5.2974 Z-0.0300
G01 X63.1469 Y-5.2287 Z-0.0300
G01 X63.3174 Y-5.1548 Z-0.0300
G01 X63.4818 Y-5.0761 Z-0.0300
G01 X63.6409 Y-4.9923 Z-0.0300
G01 X63.7940 Y-4.9040 Z-0.0300
G01 X63.9417 Y-4.8110 Z-0.0300
G01 X64.0839 Y-4.7136 Z-0.0300
G01 X64.2209 Y-4.6117 Z-0.0300
G01 X64.3523 Y-4.5059 Z-0.0300
G01 X64.4786 Y-4.3958 Z-0.0300
G01 X64.5994 Y-4.2820 Z-0.0300
G01 X64.7152 Y-4.1644 Z-0.0300
G01 X64.8261 Y-4.0429 Z-0.0300
G01 X64.9317 Y-3.9182 Z-0.0300
G01 X65.0324 Y-3.7900 Z-0.0300
G01 X65.1283 Y-3.6585 Z-0.0300
G01 X65.2193 Y-3.5239 Z-0.0300
G01 X65.3057 Y-3.3862 Z-0.0300
G01 X65.3873 Y-3.2460 Z-0.0300
G01 X65.4642 Y-3.1029 Z-0.0300
G01 X65.5367 Y-2.9573 Z-0.0300
G01 X65.6046 Y-2.8092 Z-0.0300
G01 X65.6681 Y-2.6588 Z-0.0300
G01 X65.7273 Y-2.5061 Z-0.0300
G01 X65.7821 Y-2.3516 Z-0.0300
G01 X65.8327 Y-2.1951 Z-0.0300
G01 X65.8792 Y-2.0369 Z-0.0300
G01 X65.9216 Y-1.8771 Z-0.0300
G01 X65.9600 Y-1.7157 Z-0.0300
G01 X65.9943 Y-1.5530 Z-0.0300
G01 X66.0248 Y-1.3890 Z-0.0300
G01 X66.0514 Y-1.2240 Z-0.0300
G01 X66.0743 Y-1.0581 Z-0.0300
G01 X66.0935 Y-0.8913 Z-0.0300
G01 X66.1091 Y-0.7238 Z-0.0300
G01 X66.1210 Y-0.5559 Z-0.0300
G01 X66.1295 Y-0.3875 Z-0.0300
G01 X66.1345 Y-0.2188 Z-0.0300
G01 X66.1362 Y-0.0500 Z-0.0300
G01 X66.1362 Y0.0499 Z-0.0300
G01 X66.1345 Y0.2187 Z-0.0300
G01 X66.1295 Y0.3874 Z-0.0300
G01 X66.1210 Y0.5557 Z-0.0300
G01 X66.1090 Y0.7238 Z-0.0300
G01 X66.0935 Y0.8912 Z-0.0300
G01 X66.0743 Y1.0580 Z-0.0300
G01 X66.0514 Y1.2240 Z-0.0300
G01 X66.0248 Y1.3890 Z-0.0300
G01 X65.9943 Y1.5529 Z-0.0300
G01 X65.9600 Y1.7156 Z-0.0300
G01 X65.9216 Y1.8769 Z-0.0300
G01 X65.8793 Y2.0368 Z-0.0300
G01 X65.8328 Y2.1950 Z-0.0300
G01 X65.7822 Y2.3515 Z-0.0300
G01 X65.7273 Y2.5061 Z-0.0300
G01 X65.6682 Y2.6586 Z-0.0300
G01 X65.6047 Y2.8090 Z-0.0300
G01 X65.5368 Y2.9571 Z-0.0300
G01 X65.4643 Y3.1027 Z-0.0300
G01 X65.3874 Y3.2458 Z-0.0300
G01 X65.3057 Y3.3862 Z-0.0300
G01 X65.2194 Y3.5238 Z-0.0300
G01 X65.1284 Y3.6583 Z-0.0300
G01 X65.0325 Y3.7898 Z-0.0300
G01 X64.9318 Y3.9180 Z-0.0300
G01 X64.8261 Y4.0429 Z-0.0300
G01 X64.7152 Y4.1643 Z-0.0300
G01 X64.5996 Y4.2819 Z-0.0300
G01 X64.4786 Y4.3958 Z-0.0300
G01 X64.3523 Y4.5059 Z-0.0300
G01 X64.2209 Y4.6117 Z-0.0300
G01 X64.0841 Y4.7135 Z-0.0300
G01 X63.9417 Y4.8110 Z-0.0300
G01 X63.7940 Y4.9040 Z-0.0300
G01 X63.6409 Y4.9924 Z-0.0300
G01 X63.4818 Y5.0761 Z-0.0300
G01 X63.3174 Y5.1548 Z-0.0300
G01 X63.1471 Y5.2287 Z-0.0300
G01 X62.9707 Y5.2974 Z-0.0300
G01 X62.7889 Y5.3608 Z-0.0300
G01 X62.6007 Y5.4189 Z-0.0300
G01 X62.4068 Y5.4714 Z-0.0300
G01 X62.2066 Y5.5182 Z-0.0300
G01 X62.0005 Y5.5592 Z-0.0300
G01 X61.7880 Y5.5943 Z-0.0300
G01 X61.5690 Y5.6233 Z-0.0300
G01 X61.3441 Y5.6462 Z-0.0300
G01 X61.1124 Y5.6626 Z-0.0300
G01 X60.8744 Y5.6725 Z-0.0300
G01 X60.6301 Y5.6759 Z-0.0300
G01 X60.0953 Y5.6439 Z-0.0300
G01 X59.5605 Y5.6125 Z-0.0300
G01 X59.0256 Y5.5819 Z-0.0300
G01 X58.4907 Y5.5519 Z-0.0300
G01 X57.9557 Y5.5225 Z-0.0300
G01 X57.4207 Y5.4938 Z-0.0300
G01 X56.8856 Y5.4658 Z-0.0300
G01 X56.3505 Y5.4384 Z-0.0300
G01 X55.8154 Y5.4117 Z-0.0300
G01 X55.2803 Y5.3857 Z-0.0300
G01 X54.7451 Y5.3603 Z-0.0300
G01 X54.2099 Y5.3355 Z-0.0300
G01 X53.6746 Y5.3115 Z-0.0300
G01 X53.1394 Y5.2881 Z-0.0300
G01 X52.6041 Y5.2653 Z-0.0300
G01 X52.0687 Y5.2433 Z-0.0300
G01 X51.5334 Y5.2219 Z-0.0300
G01 X50.9980 Y5.2011 Z-0.0300
G01 X50.4626 Y5.1810 Z-0.0300
G01 X49.9272 Y5.1616 Z-0.0300
G01 X49.3917 Y5.1428 Z-0.0300
G01 X48.8562 Y5.1248 Z-0.0300
G01 X48.3208 Y5.1073 Z-0.0300
G01 X47.7852 Y5.0905 Z-0.0300
G01 X47.2497 Y5.0744 Z-0.0300
G01 X46.7141 Y5.0590 Z-0.0300
G01 X46.1785 Y5.0442 Z-0.0300
G01 X45.6429 Y5.0301 Z-0.0300
G01 X45.1073 Y5.0166 Z-0.0300
G01 X44.5717 Y5.0038 Z-0.0300
G01 X44.0361 Y4.9916 Z-0.0300
G01 X43.5004 Y4.9802 Z-0.0300
G01 X42.9648 Y4.9694 Z-0.0300
G01 X42.4290 Y4.9592 Z-0.0300
G01 X41.8933 Y4.9497 Z-0.0300
G01 X41.3576 Y4.9409 Z-0.0300
G01 X40.8219 Y4.9327 Z-0.0300
G01 X40.2862 Y4.9253 Z-0.0300
G01 X39.7505 Y4.9184 Z-0.0300
G01 X39.2147 Y4.9122 Z-0.0300
G01 X38.6789 Y4.9067 Z-0.0300
G01 X38.1432 Y4.9019 Z-0.0300
G01 X37.6074 Y4.8977 Z-0.0300
G01 X37.0716 Y4.8942 Z-0.0300
G01 X36.5359 Y4.8913 Z-0.0300
G01 X36.0001 Y4.8891 Z-0.0300
G01 X35.4643 Y4.8876 Z-0.0300
G01 X34.9285 Y4.8867 Z-0.0300
G01 X34.3927 Y4.8865 Z-0.0300
G01 X33.8570 Y4.8870 Z-0.0300
G01 X33.3212 Y4.8881 Z-0.0300
G01 X32.7854 Y4.8899 Z-0.0300
G01 X32.2496 Y4.8923 Z-0.0300
G01 X31.7138 Y4.8954 Z-0.0300
G01 X31.1781 Y4.8992 Z-0.0300
G01 X30.6424 Y4.9037 Z-0.0300
G01 X30.1066 Y4.9087 Z-0.0300
G01 X29.5709 Y4.9145 Z-0.0300
G01 X29.0351 Y4.9209 Z-0.0300
G01 X28.4993 Y4.9280 Z-0.0300
G01 X27.9636 Y4.9357 Z-0.0300
G01 X27.4279 Y4.9442 Z-0.0300
G01 X26.8922 Y4.9532 Z-0.0300
G01 X26.3565 Y4.9630 Z-0.0300
G01 X25.8208 Y4.9734 Z-0.0300
G01 X25.2851 Y4.9844 Z-0.0300
G01 X24.7495 Y4.9962 Z-0.0300
G01 X24.2139 Y5.0085 Z-0.0300
G01 X23.6783 Y5.0216 Z-0.0300
G01 X23.1426 Y5.0353 Z-0.0300
G01 X22.6070 Y5.0497 Z-0.0300
G01 X22.0715 Y5.0647 Z-0.0300
G01 X21.5359 Y5.0804 Z-0.0300
G01 X21.0004 Y5.0968 Z-0.0300
G01 X20.4649 Y5.1138 Z-0.0300
G01 X19.9294 Y5.1315 Z-0.0300
G01 X19.3939 Y5.1498 Z-0.0300
G01 X18.8585 Y5.1688 Z-0.0300
G01 X18.3231 Y5.1885 Z-0.0300
G01 X17.7877 Y5.2088 Z-0.0300
G01 X17.2523 Y5.2299 Z-0.0300
G01 X16.7169 Y5.2515 Z-0.0300
G01 X16.1816 Y5.2738 Z-0.0300
G01 X15.6463 Y5.2968 Z-0.0300
G01 X15.1111 Y5.3205 Z-0.0300
G01 X14.5759 Y5.3448 Z-0.0300
G01 X14.0407 Y5.3698 Z-0.0300
G01 X13.5055 Y5.3954 Z-0.0300
G01 X12.9703 Y5.4217 Z-0.0300
G01 X12.4352 Y5.4487 Z-0.0300
G01 X11.9002 Y5.4763 Z-0.0300
G01 X11.3651 Y5.5046 Z-0.0300
G01 X10.8301 Y5.5335 Z-0.0300
G01 X10.2951 Y5.5631 Z-0.0300
G01 X9.7602 Y5.5934 Z-0.0300
G01 X9.2253 Y5.6243 Z-0.0300
G01 X8.6905 Y5.6559 Z-0.0300
G01 X8.1557 Y5.6882 Z-0.0300
G01 X7.6209 Y5.7211 Z-0.0300
G01 X7.0863 Y5.7547 Z-0.0300
G01 X6.7721 Y5.7512 Z-0.0300
G01 X6.4660 Y5.7411 Z-0.0300
G01 X6.1682 Y5.7244 Z-0.0300
G01 X5.8785 Y5.7013 Z-0.0300
G01 X5.5973 Y5.6719 Z-0.0300
G01 X5.3239 Y5.6363 Z-0.0300
G01 X5.0587 Y5.5947 Z-0.0300
G01 X4.8012 Y5.5472 Z-0.0300
G01 X4.5517 Y5.4939 Z-0.0300
G01 X4.3099 Y5.4350 Z-0.0300
G01 X4.0759 Y5.3707 Z-0.0300
G01 X3.8493 Y5.3011 Z-0.0300
G01 X3.6301 Y5.2261 Z-0.0300
G01 X3.4184 Y5.1462 Z-0.0300
G01 X3.2142 Y5.0614 Z-0.0300
G01 X3.0169 Y4.9716 Z-0.0300
G01 X2.8269 Y4.8773 Z-0.0300
G01 X2.6441 Y4.7785 Z-0.0300
G01 X2.4680 Y4.6752 Z-0.0300
G01 X2.2991 Y4.5679 Z-0.0300
G01 X2.1367 Y4.4562 Z-0.0300
G01 X1.9811 Y4.3407 Z-0.0300
G01 X1.8323 Y4.2215 Z-0.0300
G01 X1.6900 Y4.0984 Z-0.0300
G01 X1.5539 Y3.9717 Z-0.0300
G01 X1.4243 Y3.8416 Z-0.0300
G01 X1.3011 Y3.7083 Z-0.0300
G01 X1.1841 Y3.5720 Z-0.0300
G01 X1.0730 Y3.4324 Z-0.0300
G01 X0.9681 Y3.2901 Z-0.0300
G01 X0.8692 Y3.1450 Z-0.0300
G01 X0.7761 Y2.9973 Z-0.0300
G01 X0.6888 Y2.8473 Z-0.0300
G01 X0.6071 Y2.6947 Z-0.0300
G01 X0.5310 Y2.5400 Z-0.0300
G01 X0.4605 Y2.3833 Z-0.0300
G01 X0.3954 Y2.2246 Z-0.0300
G01 X0.3358 Y2.0644 Z-0.0300
G01 X0.2812 Y1.9022 Z-0.0300
G01 X0.2320 Y1.7386 Z-0.0300
G01 X0.1877 Y1.5736 Z-0.0300
G01 X0.1486 Y1.4074 Z-0.0300
G01 X0.1143 Y1.2401 Z-0.0300
G01 X0.0849 Y1.0719 Z-0.0300
G01 X0.0603 Y0.9028 Z-0.0300
G01 X0.0403 Y0.7331 Z-0.0300
G01 X0.0249 Y0.5629 Z-0.0300
G01 X0.0140 Y0.3920 Z-0.0300
G01 X0.0075 Y0.2210 Z-0.0300
G01 X0.0054 Y0.0499 Z-0.0300
G01 X0.0054 Y-0.0499 Z-0.0300
( Center groove for splitboard                                                 )
G00 X0.0054 Y-0.0499 Z0.2000
G00 X0.2524 Y-0.0335 Z0.2000
G01 X0.2524 Y-0.0335 Z-0.0300
G01 X0.2524 Y-0.0335 Z-0.0300
G01 X0.2524 Y0.0335 Z-0.0300
G01 X65.8892 Y0.0335 Z-0.0300
G01 X65.8892 Y-0.0335 Z-0.0300
G01 X0.2524 Y-0.0335 Z-0.0300
G01 X0.0054 Y-0.2805 Z-0.0300
G01 X0.0054 Y-0.2805 Z-0.0300
G01 X0.0054 Y0.2805 Z-0.0300
G01 X66.1362 Y0.2805 Z-0.0300
G01 X66.1362 Y-0.2805 Z-0.0300
G01 X0.0054 Y-0.2805 Z-0.0300
M05
G00 X0.0054 Y-0.2805 Z0.2000
G00 X0.0000 Y0.0000 Z0.2000
G49
G01 G55 Z0 F20
G00 G55 X0 Y0 Z0
M30
