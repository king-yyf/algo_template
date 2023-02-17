# 算法模板整理

<table>
	<tr>
        <th>分类</th>
        <th>算法</th>
        <th>模板代码及文档</th>
        <th>示例代码或博客</th>
    </tr>
    <tr>
        <td rowspan="9">数据结构</td>
        <td>一维树状数组</td>
        <td><a href="./data_structure/FenwickTree.md">FenwickTree</a></td>
        <td><a href="./data_structure/FenwickTree.cpp">FenwickTree.cpp</a></td>
    </tr>
    <tr>
        <td>二维树状数组</td>
        <td><a href="./data_structure/FenwickTree2D.md">FenwickTree2D</a></td>
        <td><a href="./data_structure/FenwickTree2D.cpp">FenwickTree2D.cpp</a></td>
    </tr>
    <tr>
        <td>区间修改,单点查询树状数组</td>
        <td><a href="./data_structure/RangeAddTree.md">RangeAddTree</a></td>
        <td><a href="./data_structure/RangeAddTree.cpp">RangeAddTree.cpp</a></td>
    </tr>
    <tr>
        <td>区间修改,区间查询树状数组</td>
        <td><a href="./data_structure/RangeSumTree.md">RangeSumTree</a></td>
        <td><a href="./data_structure/RangeSumTree.cpp">RangeSumTree.cpp</a></td>
    </tr>
    <tr>
        <td>ST 表</td>
        <td><a href="./data_structure/STTable.md">ST Table</a></td>
        <td><a href="https://king-yyf.github.io/2021/05/st_table/">ST Table</a></td>
    </tr>
    <tr>
        <td>线段树</td>
        <td><a href="./data_structure/SegmentTree.md">SegmentTree</a></td>
        <td><a href="./data_structure/SegmentTree.cpp">线段树</a></td>
    </tr>
    <tr>
        <td>懒标记线段树</td>
        <td><a href="./data_structure/LazySegTree.md">LazySegmentTree</a></td>
        <td><a href="./data_structure/LazySegTree.cpp">LazySegTree.cpp</a></td>
    </tr>
    <tr>
        <td>字符串字典树</td>
        <td><a href="./data_structure/StringTrie.md">字符串字典树</a></td>
        <td><a href="https://king-yyf.github.io/2022/04/string/">字符串字典树</a></td>
    </tr>
    <tr>
        <td>01字典树/可持久化01字典树</td>
        <td><a href="./data_structure/XORTrie.md">01字典树</a></td>
        <td><a href="https://king-yyf.github.io/2021/04/xor_trie/">异或与字典树</a></td>
    </tr>
    <tr>
        <td rowspan="9">字符串</td>
        <td>字符串哈希</td>
        <td><a href="./string/StringHash.md">字符串哈希</a></td>
        <td><a href="./string/StringHash.cpp">StringHash.cpp</a></td>
    </tr>
    <tr>
        <td>manacher算法</td>
        <td><a href="./string/string.md">manacher算法</a></td>
        <td><a href="https://king-yyf.github.io/2022/04/string/">manacher算法</a></td>
    </tr>
    <tr>
        <td>kmp算法</td>
        <td><a href="./string/string.md">kmp算法</a></td>
        <td><a href="https://king-yyf.github.io/2022/04/string/">kmp算法</a></td>
    </tr>
     <tr>
        <td>z_function</td>
        <td><a href="./string/string.md">z_function</a></td>
        <td><a href="https://king-yyf.github.io/2022/04/string/">z_function</a></td>
    </tr>
    <tr>
        <td>后缀数组</td>
        <td><a href="./string/string.md">后缀数组</a></td>
        <td><a href="https://king-yyf.github.io/2022/04/string/">后缀数组</a></td>
    </tr>
    <tr>
        <td>回文树/回文自动机</td>
        <td><a href="./string/string.md">回文自动机</a></td>
        <td><a href="https://king-yyf.github.io/2022/04/string/">回文自动机</a></td>
    </tr>
    <tr>
        <td>后缀自动机</td>
        <td><a href="./string/string.md">后缀自动机</a></td>
        <td><a href="https://king-yyf.github.io/2022/04/string/">后缀自动机</a></td>
    </tr>
    <tr>
        <td>lyndon分解</td>
        <td><a href="./string/string.md">lyndon分解</a></td>
        <td><a href="https://king-yyf.github.io/2022/04/string/">lyndon分解</a></td>
    </tr>
    <tr>
        <td>最小表示法</td>
        <td><a href="./string/string.md">最小表示法</a></td>
        <td><a href="https://king-yyf.github.io/2022/04/string/">最小表示法</a></td>
    </tr>
     <tr>
        <td rowspan="14">图论</td>
        <td>Dijkstra最短路</td>
        <td><a href="./graph/Dijkstra.md">Dijkstra最短路</a></td>
        <td><a href="./graph/Dijkstra.cpp">Dijkstra最短路</a></td>
    </tr>
    <tr>
        <td>01bfs</td>
        <td><a href="./graph/BinaryBFS.md">01bfs</a></td>
        <td><a href="./graph/BFS01.cpp/">01bfs</a></td>
    </tr>
     <tr>
        <td>二分图判定</td>
        <td><a href="./graph/Bigraph.md">二分图判定</a></td>
        <td><a href="https://king-yyf.github.io/2022/09/bigraph/">二分图判定</a></td>
    </tr>
    <tr>
        <td>二分图匹配</td>
        <td><a href="./graph/BigraphMatch.md">二分图匹配</a></td>
        <td><a href="https://king-yyf.github.io/2022/09/bigraph/">二分图匹配</a></td>
    </tr>
    <tr>
        <td>强连通分量</td>
        <td><a href="./graph/StronglyConnectedComponents.md">强连通分量</a></td>
        <td><a href="https://king-yyf.github.io/2023/02/scc/">强连通分量</a></td>
    </tr>
    <tr>
        <td>无向图的桥与边双连通分量</td>
        <td><a href="./graph/edcc.md">无向图的桥与边双连通分量</a></td>
        <td><a href="https://king-yyf.github.io/2023/02/scc/">无向图的桥与边双连通分量</a></td>
    </tr>
    <tr>
        <td>无向图的割点与点双连通分量</td>
        <td><a href="./graph/vdcc.md">无向图的割点与点双连通分量</a></td>
        <td><a href="https://king-yyf.github.io/2023/02/scc/">无向图的割点与点双连通分量</a></td>
    </tr>
    <tr>
        <td>spfa找负环</td>
        <td><a href="./graph/SPFA.md">spfa找负环</a></td>
        <td><a href="./graph/SPFA.cpp">spfa找负环</a></td>
    </tr>
    <tr>
        <td>差分约束问题</td>
        <td><a href="./graph/DiffCts.md">差分约束问题</a></td>
        <td><a href="./graph/DiffCts.cpp">差分约束问题</a></td>
    </tr>
    <tr>
        <td>最小生成树</td>
        <td><a href="./graph/SPFA.md">最小生成树</a></td>
        <td><a href="./graph/SPFA.cpp">最小生成树</a></td>
    </tr>
    <tr>
        <td>最短路径树</td>
        <td><a href="./graph/SPFA.md">最短路径树</a></td>
        <td><a href="./graph/SPFA.cpp">最短路径树</a></td>
    </tr>
    <tr>
        <td>k短路</td>
        <td><a href="./graph/SPFA.md">k短路</a></td>
        <td><a href="./graph/SPFA.cpp">k短路</a></td>
    </tr>
    <tr>
        <td>最大网络流</td>
        <td><a href="./graph/SPFA.md">最大网络流</a></td>
        <td><a href="./graph/SPFA.cpp">最大网络流</a></td>
    </tr>
    <tr>
        <td>最小费用流</td>
        <td><a href="./graph/SPFA.md">最小费用流</a></td>
        <td><a href="./graph/SPFA.cpp">最小费用流</a></td>
    </tr>
     <tr>
        <td rowspan="7">数学/数论</td>
        <td>modint</td>
        <td><a href="./math/modint.md">modint</a></td>
        <td><a href="./math/modint.cpp">modint</a></td>
    </tr>
    <tr>
        <td>质数</td>
        <td><a href="./graph/SPFA.md">质数</a></td>
        <td><a href="./graph/SPFA.cpp">质数</a></td>
    </tr>
    <tr>
        <td>矩阵快速幂</td>
        <td><a href="./graph/SPFA.md">矩阵快速幂</a></td>
        <td><a href="./graph/SPFA.cpp">矩阵快速幂</a></td>
    </tr>
    <tr>
        <td>容斥原理</td>
        <td><a href="./graph/SPFA.md">容斥原理</a></td>
        <td><a href="./graph/SPFA.cpp">容斥原理</a></td>
    </tr>
    <tr>
        <td>排列组合</td>
        <td><a href="./graph/SPFA.md">排列组合</a></td>
        <td><a href="./graph/SPFA.cpp">排列组合</a></td>
    </tr>
    <tr>
        <td>快速沃尔什变换</td>
        <td><a href="./graph/SPFA.md">快速沃尔什变换</a></td>
        <td><a href="./graph/SPFA.cpp">快速沃尔什变换</a></td>
    </tr>
    <tr>
        <td>高精度运算</td>
        <td><a href="./graph/SPFA.md">高精度运算</a></td>
        <td><a href="./graph/SPFA.cpp">高精度运算</a></td>
    </tr>
    <tr>
        <td rowspan="7">树上算法</td>
        <td>最近公共祖先(LCA)</td>
        <td><a href="./tree/lca.md">最近公共祖先(LCA)</a></td>
        <td><a href="https://king-yyf.github.io/2022/06/lca/">最近公共祖先(LCA)</a></td>
    </tr>
    <tr>
        <td>带权LCA</td>
        <td><a href="./tree/weighted_lca.md">带权LCA</a></td>
        <td><a href="./tree/weighted_lca.cpp">带权LCA</a></td>
    </tr>
    <tr>
        <td>树的直径</td>
        <td><a href="./tree/SPFA.md">树的直径</a></td>
        <td><a href="./tree/SPFA.cpp">树的直径</a></td>
    </tr>
    <tr>
        <td>树链剖分</td>
        <td><a href="./tree/hld.md">树链剖分</a></td>
        <td><a href="./tree/hld.cpp">树链剖分</a></td>
    </tr>
    <tr>
        <td>点分治</td>
        <td><a href="./tree/hld.md">点分治</a></td>
        <td><a href="./tree/hld.cpp">点分治</a></td>
    </tr>
    <tr>
        <td>点分树</td>
        <td><a href="./tree/SPFA.md">点分树</a></td>
        <td><a href="./tree/SPFA.cpp">点分树</a></td>
    </tr>
     <tr>
        <td>树哈希</td>
        <td><a href="./tree/SPFA.md">树哈希</a></td>
        <td><a href="./tree/SPFA.cpp">树哈希</a></td>
    </tr>
     <tr>
        <td rowspan="4">位运算</td>
        <td>bitset</td>
        <td><a href="./graph/Dijkstra.md">bitset</a></td>
        <td><a href="./graph/Dijkstra.cpp">bitset</a></td>
    </tr>
     <tr>
        <td>位运算</td>
        <td><a href="./bits/bitwise.md">位运算</a></td>
        <td><a href="./bits/bitwise.cpp">位运算</a></td>
    </tr>
    <tr>
        <td>异或线形基</td>
        <td><a href="./graph/SPFA.md">异或线形基</a></td>
        <td><a href="./graph/SPFA.cpp">异或线形基</a></td>
    </tr>
    <tr>
        <td>子集卷积</td>
        <td><a href="./graph/SPFA.md">子集卷积</a></td>
        <td><a href="./graph/SPFA.cpp">子集卷积</a></td>
    </tr>
    <tr>
        <td rowspan="7">常用算法</td>
        <td>并查集</td>
        <td><a href="./base_algo/disjointSet.md">并查集</a></td>
        <td><a href="./base_algo/disjointSet.md">并查集</a></td>
    </tr>
    <tr>
        <td>二分算法</td>
        <td><a href="./base_algo/binary_search.md">二分算法</a></td>
        <td><a href="./base_algo/binary_search.md">二分算法</a></td>
    </tr>
    <tr>
        <td>二维前缀和</td>
        <td><a href="./base_algo/prefixSum2d.md">二维前缀和</a></td>
        <td><a href="./base_algo/prefixSum2d.md">二维前缀和</a></td>
    </tr>
    <tr>
        <td>二维差分</td>
        <td><a href="./base_algo/PrefixDiff2D.md">二维差分</a></td>
        <td><a href="./base_algo/PrefixDiff2D.md">二维差分</a></td>
    </tr>
    <tr>
        <td>单调栈</td>
        <td><a href="./base_algo/stack.md">单调栈</a></td>
        <td><a href="./base_algo/stack.cpp">单调栈</a></td>
    </tr>
    <tr>
        <td>单调队列</td>
        <td><a href="./base_algo/queue.md">单调队列</a></td>
        <td><a href="https://king-yyf.github.io/2021/03/que_stk/">单调队列</a></td>
    </tr>
    <tr>
        <td>维护区间(交/并)</td>
        <td><a href="./base_algo/interval.md">维护区间(交/并)</a></td>
        <td><a href="./base_algo/interval.cpp">维护区间(交/并)</a></td>
    </tr>
    <tr>
        <td rowspan="5">杂项</td>
        <td>莫队</td>
        <td><a href="./graph/Dijkstra.md">莫队</a></td>
        <td><a href="./graph/Dijkstra.cpp">莫队</a></td>
    </tr>
    <tr>
        <td>树上莫队</td>
        <td><a href="./graph/SPFA.md">树上莫队</a></td>
        <td><a href="./graph/SPFA.cpp">树上莫队</a></td>
    </tr>
    <tr>
        <td>数组离散化</td>
        <td><a href="./misc/Discrete.md">数组离散化</a></td>
        <td><a href="./misc/Discrete.md">数组离散化</a></td>
    </tr>
    <tr>
        <td>在线前缀最值</td>
        <td><a href="./misc/PrefixMax.md">在线前缀最值</a></td>
        <td><a href="./misc/PrefixMax.md">在线前缀最值</a></td>
    </tr>
    <tr>
        <td>数组哈希</td>
        <td><a href="./misc/array_hash.md">数组哈希</a></td>
        <td><a href="./misc/array_hash.md">数组哈希</a></td>
    </tr>
</table>
