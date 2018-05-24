# Builder 

生成器模式是一种对象构建模式。它可以将复杂对象的建造过程抽象出来(抽象类别),使这个抽象过程的不同实现方法可以构造出不同表现(属性)的对象。  

抽象工厂模式与生成器相似,因为它也可以创建复杂对象。主要的区别是生成器模式着重于一步步构造一个复杂对象。而抽象工厂模式着重于多个系列的产品对象(简单的或是复杂的)。生成器在最后的一步返回产品,而对于抽象工厂来说,产品是立即返回的。   

建造者模式包含如下角色:   

- Builder: 抽象建造者  
- ConcreteBuilder: 具体建造者  
- Director: 指挥者  
- Product: 产品角色  

建造者模式主要是用于创建一些复杂的对象, 这些对象内部构建间的建造顺序是稳定的, 但是对象内部的构建通常面临着复杂的变化。   

建造者模式的好处就是使得建造代码与表示代码分离, 由于建造者隐藏了该产品是如何组装的, 所以若需要改变一个产品的内部表示,只需要再定义一个具体的建造者就可以了.   

## Intent

将一个复杂对象的构造函数分离, 这样就可以充分利用这些分离后的组件了.   

解析一个复杂的表示, create one of several targets.   

## Problem   

一个应用程序需要创建一个复杂的集合. 组成这个集合的成员在辅助存储器中, 并且一个复杂对象需要在主存储器中 build.     

An application needs to create the elements of a complex aggregate. The specification for the aggregate exists on secondary storage and one of many representations needs to be built in primary storage.

## Discussion

Separate the algorithm for interpreting (i.e. reading and parsing) a stored persistence mechanism (e.g. RTF files) from the algorithm for building and representing one of many target products (e.g. ASCII, TeX, text widget). The focus/distinction is on creating complex aggregates.

The "director" invokes "builder" services as it interprets the external format. The "builder" creates part of the complex object each time it is called and maintains all intermediate state. When the product is finished, the client retrieves the result from the "builder".

Affords finer control over the construction process. Unlike creational patterns that construct products in one shot, the Builder pattern constructs the product step by step under the control of the "director".

## Structure

The Reader encapsulates the parsing of the common input. The Builder hierarchy makes possible the polymorphic creation of many peculiar representations or targets.

![Scheme of Builder](../../snapshots/Builder.png)    

## Example

The Builder pattern separates the construction of a complex object from its representation so that the same construction process can create different representations. This pattern is used by fast food restaurants to construct children's meals. Children's meals typically consist of a main item, a side item, a drink, and a toy (e.g., a hamburger, fries, Coke, and toy dinosaur). Note that there can be variation in the content of the children's meal, but the construction process is the same. Whether a customer orders a hamburger, cheeseburger, or chicken, the process is the same. The employee at the counter directs the crew to assemble a main item, side item, and toy. These items are then placed in a bag. The drink is placed in a cup and remains outside of the bag. This same process is used at competing restaurants.

![Example of Builder](../../snapshots/Builder_example1.png)    

## Check list

Decide if a common input and many possible representations (or outputs) is the problem at hand.
Encapsulate the parsing of the common input in a Reader class.
Design a standard protocol for creating all possible output representations. Capture the steps of this protocol in a Builder interface.
Define a Builder derived class for each target representation.
The client creates a Reader object and a Builder object, and registers the latter with the former.
The client asks the Reader to "construct".
The client asks the Builder to return the result.

## Rules of thumb - 经验法则   

Sometimes creational patterns are complementary: Builder can use one of the other patterns to implement which components get built. Abstract Factory, Builder, and Prototype can use Singleton in their implementations.
Builder focuses on constructing a complex object step by step. Abstract Factory emphasizes a family of product objects (either simple or complex). Builder returns the product as a final step, but as far as the Abstract Factory is concerned, the product gets returned immediately.
Builder often builds a Composite.
Often, designs start out using Factory Method (less complicated, more customizable, subclasses proliferate) and evolve toward Abstract Factory, Prototype, or Builder (more flexible, more complex) as the designer discovers where more flexibility is needed.