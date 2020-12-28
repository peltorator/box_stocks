from manimlib.imports import *


class MyScene(Scene):

    objects = []

    def wanish(self):
        self.play(*[FadeOut(obj) for obj in self.objects])
        self.objects.clear()

    def saveObject(self, obj):
        self.objects.append(obj)

    def saveObjectList(self, objList):
        for obj in objList:
            self.objects.append(obj)


class ExplanationScene(MyScene):
    
    def construct(self):
        logo = ImageMobject('logo.png')
        logo.scale(1.5)
        logo.shift(DOWN)
        contacts = TextMobject(r'Егор Горбачев (tg: @peltorator)\\Руководитель: Дмитрий Шалымов')
        contacts.next_to(logo, UP)
        title = TextMobject('Интернет-магазин и задача о рюкзаке')
        self.saveObject(title)
        title.next_to(contacts, UP)
        
        self.play(Write(title))
        self.play(Write(contacts))
        self.play(FadeIn(logo))
        self.wait(1)
        self.play(ApplyMethod(title.to_edge, UP), FadeOut(contacts), FadeOut(logo))

        scale = 0.5
        circle = Circle(color=YELLOW, fill_color=YELLOW, fill_opacity=0.5)
        self.saveObject(circle)
        circle.scale(scale)
        circle.to_edge(LEFT)
        circle.shift(UP)

        square = Square(color=YELLOW, fill_color=YELLOW, fill_opacity=0.5)
        self.saveObject(square)
        square.scale(scale)
        square.to_edge(LEFT)
        square.next_to(circle, DOWN)

        triangle = Polygon(np.array([-1, 0, 0]),np.array([0, 1.74, 0]),np.array([1, 0, 0]), color=YELLOW, fill_color=YELLOW, fill_opacity=0.5)
        self.saveObject(triangle)
        triangle.scale(scale)
        triangle.to_edge(LEFT)
        triangle.next_to(square, DOWN)

        self.wait(2.5)
        self.play(FadeIn(circle), run_time=0.3)
        self.play(FadeIn(square), run_time=0.3)
        self.play(FadeIn(triangle), run_time=0.3)

        bigRightLine = Line(np.array([6.5, 2.5, 0]),np.array([6.5, 0, 0]), color=BLUE)
        bigDownLine = Line(np.array([6.5, 0, 0]), np.array([4, 0, 0]), color=BLUE)
        bigLeftLine = Line(np.array([4, 0, 0]), np.array([4, 2.5, 0]), color=BLUE)
        self.saveObjectList([bigRightLine, bigDownLine, bigLeftLine])

        self.play(FadeIn(bigRightLine), FadeIn(bigDownLine), FadeIn(bigLeftLine), run_time=0.3)

        mediumRightLine = Line(np.array([6.5, -0.5, 0]),np.array([6.5, -2, 0]), color=RED)
        mediumDownLine = Line(np.array([6.5, -2, 0]), np.array([5, -2, 0]), color=RED)
        mediumLeftLine = Line(np.array([5, -2, 0]), np.array([5, -0.5, 0]), color=RED)
        self.saveObjectList([mediumRightLine, mediumDownLine, mediumLeftLine])
        
        self.play(FadeIn(mediumRightLine), FadeIn(mediumDownLine), FadeIn(mediumLeftLine), run_time=0.3)

        smallRightLine = Line(np.array([6.5, -2.5, 0]),np.array([6.5, -3.5, 0]), color=GREEN)
        smallDownLine = Line(np.array([6.5, -3.5, 0]), np.array([5.5, -3.5, 0]), color=GREEN)
        smallLeftLine = Line(np.array([5.5, -3.5, 0]), np.array([5.5, -2.5, 0]), color=GREEN)
        self.saveObjectList([smallRightLine, smallDownLine, smallLeftLine])

        self.play(FadeIn(smallRightLine), FadeIn(smallDownLine), FadeIn(smallLeftLine), run_time=0.3)

        w = TexMobject('w')
        v = TexMobject('v')
        c = TexMobject('c')
        w.scale(0.8)
        v.scale(0.8)
        c.scale(0.8)
        w.move_to(circle.get_center())
        v.move_to(circle.get_center())
        c.move_to(circle.get_center())
        w.next_to(circle, RIGHT)
        v.next_to(circle, RIGHT)
        c.next_to(circle, RIGHT)
        w.shift(UP * 0.3)
        c.shift(DOWN * 0.3)
        self.wait(2)
        self.play(Write(w), run_time=1)
        self.play(Write(v), run_time=1)
        self.play(Write(c), run_time=1)

        maxw = TexMobject('maxw')
        boxv = TexMobject('boxv')
        boxc = TexMobject('boxc')
        maxw.scale(0.8)
        boxv.scale(0.8)
        boxc.scale(0.8)
        maxw.move_to(bigLeftLine.get_center())
        boxv.move_to(bigLeftLine.get_center())
        boxc.move_to(bigLeftLine.get_center())
        maxw.next_to(bigLeftLine, LEFT)
        boxv.next_to(bigLeftLine, LEFT)
        boxc.next_to(bigLeftLine, LEFT)
        maxw.shift(UP * 0.8)
        boxc.shift(DOWN * 0.8)
        self.wait(3)
        self.play(Write(maxw), run_time=2)
        self.play(Write(boxv), run_time=1.5)
        self.play(Write(boxc), run_time=1.5)


        sumW = TexMobject('\sum w_i')
        self.saveObject(sumW)
        lessBoxW = TexMobject('\le maxw')
        self.saveObject(lessBoxW)
        lessBoxW.next_to(sumW, RIGHT)
        group = VGroup(sumW, lessBoxW)
        group.move_to(ORIGIN)
        group.next_to(title, DOWN)
        self.wait(5)
        self.play(ReplacementTransform(w, sumW), ReplacementTransform(maxw, lessBoxW))

        sumV = TexMobject('\sum v_i')
        self.saveObject(sumV)
        lessBoxV = TexMobject('\le boxv')
        self.saveObject(lessBoxV)
        lessBoxV.next_to(sumV, RIGHT)
        group2 = VGroup(sumV, lessBoxV)
        group2.move_to(ORIGIN)
        group2.next_to(group, DOWN)
        self.wait(4.5)
        self.play(ReplacementTransform(v, sumV), ReplacementTransform(boxv, lessBoxV))

        finalCost = TexMobject('FinalCost = ')
        self.saveObject(finalCost)
        sumC = TexMobject('\sum c_i')
        self.saveObject(sumC)
        sumC.next_to(finalCost, RIGHT)
        plusBoxC = TexMobject('+ \sum boxc_i')
        self.saveObject(plusBoxC)
        plusBoxC.next_to(sumC, RIGHT)
        group3 = VGroup(finalCost, sumC, plusBoxC)
        group3.move_to(ORIGIN)
        group3.next_to(group2, DOWN)
        self.wait(3)
        self.play(Write(finalCost), run_time=1.5)
        self.play(ReplacementTransform(c, sumC), run_time=1.5)
        self.play(ReplacementTransform(boxc, plusBoxC), run_time=1.5)

        self.wait(4)
        self.play(Indicate(sumC))
        self.wait(3.5)
        self.play(Indicate(plusBoxC))

        itemsGroup = VGroup(circle, square, triangle)
        newGroup = itemsGroup.copy()
        newGroup.scale(0.7)
        newGroup.move_to(np.array([5.25, 1.25, 0]))
        self.wait(3.5)
        self.play(Transform(itemsGroup, newGroup))

        self.wait(1)
        self.wanish()


class SolutionScene(MyScene):

    def construct(self):
        self.wait(7)
        self.badSolutions()
        self.wait(2.5)
        self.wanish()
        self.wait(2)
        self.mySolution()
        self.wait(7.5)
        self.wanish()
        self.timeComplexity()
        self.wait(15)
        self.wanish()

    def badSolutions(self):
        bigWeight = TextMobject(r'Веса, объемы и стоимости в асимптотике $\left(Weight \approx 10^9\right)$')
        self.saveObject(bigWeight)
        bigWeight.shift(UP)
        self.play(Write(bigWeight))
        self.wait(2)
        self.play(Indicate(bigWeight, color=RED))

        self.wait(4)

        greedy = TextMobject('Жадность')
        self.saveObject(greedy)
        greedy.shift(DOWN)
        self.play(Write(greedy))
        self.wait(1.5)
        self.play(Indicate(greedy, color=RED))

        self.wait(4)

        L1 = Line(np.array([-4, 3, 0]), np.array([4, -3, 0]), color=RED)
        self.saveObject(L1)
        L2 = Line(np.array([4, 3, 0]), np.array([-4, -3, 0]), color=RED)
        self.saveObject(L2)
        self.play(FadeIn(L1), FadeIn(L2))

    def mySolution(self):
        title = TextMobject('Динамика по подмножествам')
        self.saveObject(title)
        title.to_edge(UP)
        self.play(Write(title))

        items = TextMobject('Заказали n предметов')
        self.saveObject(items)
        items.next_to(title, DOWN)
        self.wait(1.5)
        self.play(Write(items))

        mask = TexMobject(r'100101\ldots011010')
        self.saveObject(mask)
        mask.next_to(items, DOWN)
        mask.shift(DOWN)
        self.wait(3)
        self.play(Write(mask))

        brace = Brace(mask, UP)
        self.saveObject(brace)
        braceText = brace.get_text('n')
        self.saveObject(braceText)
        self.play(GrowFromCenter(brace), Write(braceText))

        dp = TextMobject('minCost[mask]')
        self.saveObject(dp)
        dp.next_to(mask, DOWN)
        dp.shift(DOWN * 0.8)
        self.wait(11)
        self.play(Write(dp))
        
        leftArrow = Arrow(DOWN * 1, DOWN * 2.2 + LEFT * 4)
        self.saveObject(leftArrow)
        boxMask = TextMobject(r'boxMask\\(предметы из текущей коробки)')
        boxMask.scale(0.8)
        self.saveObject(boxMask)
        boxMask.next_to(leftArrow, DOWN)
        boxMask.shift(LEFT * 1.5)
        self.wait(4.5)
        self.play(GrowArrow(leftArrow), Write(boxMask))

        rightArrow = Arrow(DOWN * 1, DOWN * 2.2 + RIGHT * 4)
        self.saveObject(rightArrow)
        otherMask = TextMobject(r'otherMask\\(все остальное)')
        otherMask.scale(0.8)
        self.saveObject(otherMask)
        otherMask.next_to(rightArrow, DOWN)
        otherMask.shift(RIGHT * 1.5)
        self.wait(4)
        self.play(GrowArrow(rightArrow), Write(otherMask))

    def timeComplexity(self):
        formula = TexMobject(r'minCost[mask] = \min_{boxMask}  cost(boxMask) + minCost[otherMask]')
        self.saveObject(formula)
        formula.scale(0.85)
        formula.shift(UP)
        self.play(Write(formula))

        time = TexMobject(r'Time = \mathbb{O}\left(3^n \cdot k\right)')
        self.saveObject(time)
        time.shift(DOWN)
        self.wait(11)
        self.play(Write(time))


class RealisationScene(MyScene):

    def construct(self):
        self.wait(18)
        self.makeScheme()
        self.wait(4)
        self.wanish()

    def makeScheme(self):
        clientRect = Rectangle(height=3.0, width=5.0)
        self.saveObject(clientRect)
        clientRect.to_edge(LEFT)
        clientRect.shift(DOWN)

        clientText = TextMobject(r'Клиент\\Графическое приложение\\C++\\Графика: SFML\\Логирование: easylogging')
        clientText.scale(0.7)
        self.saveObject(clientText)
        clientText.move_to(clientRect.get_center())
        
        self.play(FadeIn(clientRect), Write(clientText))


        serverRect = Rectangle(height=2.0, width=4.5)
        self.saveObject(serverRect)
        serverRect.shift(DOWN + RIGHT * 3)

        serverText = TextMobject(r'Сервер\\Python\\Логирование: easylogging')
        serverText.scale(0.7)
        self.saveObject(serverText)
        serverText.move_to(serverRect.get_center())

        clientServerArrow = Arrow(DOWN + 1.7 * LEFT, DOWN + RIGHT)
        self.saveObject(clientServerArrow)

        http = TextMobject(r'HTTP\\cpp-httplib')
        http.scale(0.7)
        self.saveObject(http)
        http.next_to(clientServerArrow, UP)
        self.wait(3)
        self.play(FadeIn(serverRect), Write(serverText), GrowArrow(clientServerArrow), Write(http))


        shopRect = Rectangle(height=2.0, width=3.0)
        self.saveObject(shopRect)
        shopRect.shift(UP * 2.5 + LEFT * 0.5)

        shopText = TextMobject(r'Магазин\\C++\\Тесты: catch2')
        shopText.scale(0.7)
        self.saveObject(shopText)
        shopText.move_to(shopRect.get_center())

        serverShopArrow = Arrow(RIGHT * 2, UP * 1.4 + LEFT * 0.5)
        self.saveObject(serverShopArrow)

        cpython = TextMobject(r'python c++\\extension')
        cpython.scale(0.5)
        self.saveObject(cpython)
        cpython.next_to(serverShopArrow, RIGHT)
        cpython.shift(LEFT * 0.7 + UP * 0.3)
        self.wait(3)
        self.play(FadeIn(shopRect), Write(shopText), GrowArrow(serverShopArrow), Write(cpython))


        dbRect = Rectangle(height=2.0, width=3.0)
        self.saveObject(dbRect)
        dbRect.shift(UP * 2.5 + RIGHT * 5)

        dbText = TextMobject(r'База данных\\sqlite\\Тесты: catch2')
        dbText.scale(0.7)
        self.saveObject(dbText)
        dbText.move_to(dbRect.get_center())

        shopDBArrow = Arrow(UP * 2.5 + RIGHT * 1, UP * 2.5 + RIGHT * 3.5)
        self.saveObject(shopDBArrow)
        self.wait(8.5)
        self.play(FadeIn(dbRect), Write(dbText), GrowArrow(shopDBArrow))


class FinalScene(Scene):

    def construct(self):
        text2 = TextMobject('Спасибо за внимание!')
        text1 = TextMobject('Внимание!!!')
        text1.next_to(text2, UP)
        text3 = TextMobject(r'Репозиторий гитхаб: github.com/Peltorator/box\_stocks')
        text3.next_to(text2, DOWN)
        self.wait(25)
        self.play(Write(text1))
        self.play(Write(text2))
        self.play(Write(text3))
        self.wait(1)
