## BP神经网络推导与实现
来源:https://blog.csdn.net/zhaomengszu/article/details/77834845

![](./doc/network.jpg)

输入层：两个神经元i1,i2 和 截距 b1
隐含层:两个神经元h1,h2 和 截距 b2
输出层:两个输出

### Step 1 前向传播 forward

输出层 -> 隐藏层

$$
net_{h1} = w_1*i_1 + w_2*i_2+b_1
$$

$$
net_{h2} = w_3*i_1 + w_4*i_2+b_1
$$

$$
out_{h1} = sigmod(net_{h_1}) = \frac{1}{1+e^{-net_{h1}}}
$$

$$
out_{h2} = \frac{1}{1+e^{-net_{h2}}}
$$
隐含层 -> 输出层
$$
net_{o1} = w_5*out_{h1}+w_6*out_{h2}+b_2

$$

$$net_{o2} = w_7*out_{h1}+w_8*out_{h2}+b_2$$

$$
out_{1} = \frac{1}{1+e^{-net_{o1}}}
$$
$$
out_{2} = \frac{1}{1+e^{-net_{o2}}}
$$
### Step2 反向传播

计算误差
$$E_{total} = \frac{1}{2}\sum {(target - output)^2}$$
这里有两个输出值:
$$
E_{total} = E_{o1} + E_{o2}
$$
隐藏层->输出层 权重更新
![](./doc/network1.jpg)

以$w_5$为例,如果我们想知道$w_5$对整体误差产生了多少影响,可以利用整体误差对$w_5$求偏导(链式法则)

 $$\frac{\alpha{E_{total}}}{\alpha{w_5}} = \frac{\alpha{E_{total}}}{\alpha{out_1}}*\frac{\alpha{out_1}}{\alpha{net_{o1}}}*\frac{\alpha{neto_1}}{\alpha{w_5}}$$

$$E_{total} = \frac{1}{2}*[(targeto_1-out_1)^2+(targeto_2-out_2)^2]$$

- $$\frac{\alpha{E_{total}}}{\alpha{out_1}} = -(targeto_1 - out_1)$$
- $$\frac{\alpha{out_1}}{\alpha{net_{o1}}} = \frac{\alpha{\frac{1}{1+e^-net_{o1}}}}{\alpha{net_{o1}}}=out_1(1-out_1)$$
- $$\frac{\alpha{net_{o1}}}{\alpha{w_5}}=out_{h1}$$
三者相乘
令$\delta_{o1}=-(traget_{o1}-out_1)*out_1(1-out_1)$
则
$$\frac{\alpha{E_{total}}}{\alpha{w_5}} = \delta_{o1}out_{h1}$$
更新权重$w_5 = w_5 - \eta*\frac{\alpha{E_{total}}}{\alpha{w_5}}$

同理可以更新隐藏层->输出层的其他权重值

隐藏层(输入层)->隐藏层权重更新

在上文计算总误差对$w_5$的偏导时，是从$out_1$->$net_{o1}$->$w_5$，但是在隐藏层之间的更新时是$out_{h1}$->$net_{h1}$->$w_1$，而$out_{h1}$会接受两个$E_{o1}$和$E_{o2}$传来的误差，所以这两个地方都要计算

![](./doc/network2.jpg)

