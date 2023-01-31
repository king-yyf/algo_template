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
        <td rowspan="8">图论</td>
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
        <td><a href="./graph/StronglyConnectedComponents.cpp">强连通分量</a></td>
    </tr>
</table>
