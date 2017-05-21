C语言产生随机数


尽管在计算机中并没有一个真正的随机数发生器，但是可以做到使产生的数字的重复率很低，以至于它们看起来是随机的。实现这一功能的程序叫做伪随机数发生器。

有关如何产生随机数的理论有许多，这里不讨论这些理论及相关的数学知识。因为讨论这一主题需要整整一本书的篇幅。这里要说的是，不管你用什么办法实现随机数发生器，你都必须给它提供一个被称为“种子(seed)”的初始值，
而且这个值最好是随机的，或者至少是伪随机的。“种子”的值通常是用快速计数寄存器或移位寄存器来生成的。

本文中，笔者将介绍c语言所提供的随机数发生器的用法。现在的c编译程序都提供了一个基于一种ANSI标准的伪随机数发生器函数，用来生成随机数。Microsoft和Borland都是通过rand()和srand()函数来支持这种标准的，
它们的工作过程如下：

    首先，给srand()提供一个“种子”，它是一个unsignde int类型，其取值范围是从0到65,535 ;
    然后，调用rand()，它会根据提供给srand()的“种子”值返回一个随机数(在0到32，767之间)；
    根据需要多次调用rand()，从而不断地得到新的随机数；
    无论什么时候，你都可以给srand()提供一个新的“种子”，从而进一步“随机化"rand()的输出结果。"


这个过程看起来很简单，问题是如果你每次调用srand()时都提供相同的“种子”值，那么你将会得到相同的“随机”数序列。例如，在以17为“种子”值调用srand()之后，在你首次调用rand()时，你将得到随机数94；在你第二次和
第三次调用rand()时，你将分别得到26，602和30，017。这些数看上去是相当随机的(尽管这只是一个很小的数据点集合)，但是，在你再次以17为“种子”值调用srand()之后，在对rand()的前三次调用中，所得到的返回值仍然是94、
26，602和30，017，并且此后得到的返回值仍然是在对rand()的第一批调用中所得到的其余的返回值。因此，只有再次给srand()提供一个随机的“种子”值，才能再次得到一个随机数。

下面的例子用一种简单而有效的办法来产生一个相当随机的“种子”值——当天的时间值。

    # include <stdlib. h>
    # include <stdio. h>
    # include <sys/types. h>
    # include <sys/timeb. h>
    void main (void){
        int i ;
        unsigned int seedVal;
        struct_timeb timeBuf ;
        _ftime (&timeBuf) ;
        seedVal = ( ( ( ( (unsigned int)timeBuf, time & 0xFFFF) +
                       (unsigned int)timeBuf, millitm) ^
                       (unsigned int)timeBuf, millitm) ;
        srand ((unsigned int)seedVal) ;
        for(i=O;i<lO;++i)
            printf (" %6d\n" ,rand ( ) ) ;
    }


上例先是调用_ftime()来检索当前时间，并把它的值存入结构成员timeBuf.time中，当前时间的值从1970年1月1日开始以秒计算。在调用了_ftime()之后，在结构timeBuf的成员millitm中还存入了在当前那一秒已经度过的毫秒数，
但在DOS中这个数字实际上是以百分之一秒来计算的。然后，把毫秒数和秒数相加，再和毫秒数进行一次异或运算。你可以对这两个结构成员施加更多的逻辑运算，以控制seedVal的取值范围，并进一步加强它的随机性，但上例所用的逻辑
运算已经足够了。

注意，在前面的例子中，rand()的输出并没有被限制在一个指定的范围内，假设你想建立一个彩票选号器，其取值范围是从1到44。你可以简单地忽略掉rand()所输出的在该范围之外的值，但这将花费许多时间去得到所需的全部(例如6个)
彩票号码。假设你已经建立了一个令你满意的随机数发生器，它所产生的随机数据范围是从0到32，767(就象前文中提到过的那样)，而你想把输出限制在1到44之间，下面的例子就说明了如何来完成这项工作：

    int i ,k ,range ;
    int rain, max ;
    double j ;
    min=1;    /* 1 is the minimum number allowed */
    max=44;    /* 44 is the maximum number allowed */
    range=max-min;    /* r is the range allowed; 1 to 44 */
    i=rand();    /* use the above example in this slot */
    /* Normalize the rand() output (scale to 0 to 1) */
    /* RAND_MAX is defined in stdlib, h */
    j= ((double)i/(double)RAND_MAX) ;
    /* Scale the output to 1 to 44 */
    i= (int)(j * (double)range) ;
    i+ =min;


上例把输出的随机数限制在1到44之间，其工作原理如下：

    得到一个在O到RAND_MAX(32，767)之间的随机数，把它除以RAND_MAX，从而产生一个在0到1之间的校正值；
    把校正值乘以所需要的范围值(在本例中为43，即44减去1)，从而产生一个在O到43之间的值；
    把该值和所要求的最小值相加，从而使该值最终落在正确的取值范围——1到44之内。

""
你可以用不同的min和max值来验证这个例子，你会发现它总是会正确地产生在新的rain和max值之间的随机数