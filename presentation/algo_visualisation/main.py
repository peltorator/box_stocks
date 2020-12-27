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
        title = TextMobject('Интернет-магазин и задача о рюкзаке')
        self.saveObject(title)
        title.to_edge(UP)

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
        square.next_to(square, DOWN)

        self.play(Write(title))
        self.play(FadeIn(circle), FadeIn(square))

        bigRightLine = Line(np.array([6.5, 2.5, 0]),np.array([6.5, 0, 0]), color=BLUE)
        bigDownLine = Line(np.array([6.5, 0, 0]), np.array([4, 0, 0]), color=BLUE)
        bigLeftLine = Line(np.array([4, 0, 0]), np.array([4, 2.5, 0]), color=BLUE)
        self.saveObjectList([bigRightLine, bigDownLine, bigLeftLine])

        self.play(FadeIn(bigRightLine), FadeIn(bigDownLine), FadeIn(bigLeftLine))

        mediumRightLine = Line(np.array([6.5, -0.5, 0]),np.array([6.5, -2, 0]), color=RED)
        mediumDownLine = Line(np.array([6.5, -2, 0]), np.array([5, -2, 0]), color=RED)
        mediumLeftLine = Line(np.array([5, -2, 0]), np.array([5, -0.5, 0]), color=RED)
        self.saveObjectList([mediumRightLine, mediumDownLine, mediumLeftLine])
        
        self.play(FadeIn(mediumRightLine), FadeIn(mediumDownLine), FadeIn(mediumLeftLine))

        smallRightLine = Line(np.array([6.5, -2.5, 0]),np.array([6.5, -3.5, 0]), color=GREEN)
        smallDownLine = Line(np.array([6.5, -3.5, 0]), np.array([5.5, -3.5, 0]), color=GREEN)
        smallLeftLine = Line(np.array([5.5, -3.5, 0]), np.array([5.5, -2.5, 0]), color=GREEN)
        self.saveObjectList([smallRightLine, smallDownLine, smallLeftLine])

        self.play(FadeIn(smallRightLine), FadeIn(smallDownLine), FadeIn(smallLeftLine))

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
        self.play(Write(w), run_time=0.3)
        self.play(Write(v), run_time=0.3)
        self.play(Write(c), run_time=0.3)

        maxw = TexMobject('maxw')
        maxv = TexMobject('maxv')
        boxc = TexMobject('boxc')
        maxw.scale(0.8)
        maxv.scale(0.8)
        boxc.scale(0.8)
        maxw.move_to(bigLeftLine.get_center())
        maxv.move_to(bigLeftLine.get_center())
        boxc.move_to(bigLeftLine.get_center())
        maxw.next_to(bigLeftLine, LEFT)
        maxv.next_to(bigLeftLine, LEFT)
        boxc.next_to(bigLeftLine, LEFT)
        maxw.shift(UP * 0.8)
        boxc.shift(DOWN * 0.8)
        self.play(Write(maxw), run_time=0.3)
        self.play(Write(maxv), run_time=0.3)
        self.play(Write(boxc), run_time=0.3)


        sumW = TexMobject('\sum w_i')
        self.saveObject(sumW)
        lessBoxW = TexMobject('\le maxw')
        self.saveObject(lessBoxW)
        lessBoxW.next_to(sumW, RIGHT)
        group = VGroup(sumW, lessBoxW)
        group.move_to(ORIGIN)
        group.next_to(title, DOWN)
        self.play(ReplacementTransform(w, sumW), ReplacementTransform(maxw, lessBoxW))

        sumV = TexMobject('\sum v_i')
        self.saveObject(sumV)
        lessBoxV = TexMobject('\le maxv')
        self.saveObject(lessBoxV)
        lessBoxV.next_to(sumV, RIGHT)
        group2 = VGroup(sumV, lessBoxV)
        group2.move_to(ORIGIN)
        group2.next_to(group, DOWN)
        self.play(ReplacementTransform(v, sumV), ReplacementTransform(maxv, lessBoxV))

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
        self.play(ReplacementTransform(c, sumC), ReplacementTransform(boxc, plusBoxC))
        self.play(Write(finalCost))

        self.play(Indicate(sumC))
        self.play(Indicate(plusBoxC))

        self.wait(1)
        self.wanish()


class SolutionScene(MyScene):

    def construct(self):
        self.wait(1)
        self.badSolutions()
        self.wait(1)
        self.wanish()
        self.mySolution()
        self.wait(1)
        self.wanish()
        self.timeComplexity()
        self.wait(1)
        self.wanish()

    def badSolutions(self):
        bigWeight = TextMobject(r'Веса, объемы и стоимости в асимптотике $\left(Weight \approx 10^9\right)$')
        self.saveObject(bigWeight)
        bigWeight.shift(UP)
        self.play(Write(bigWeight))
        self.play(Indicate(bigWeight, color=RED))

        self.wait(1)

        greedy = TextMobject('Жадность')
        self.saveObject(greedy)
        greedy.shift(DOWN)
        self.play(Write(greedy))
        self.play(Indicate(greedy, color=RED))

        self.wait(1)

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
        self.play(Write(items))

        mask = TexMobject(r'100101\ldots011010')
        self.saveObject(mask)
        mask.next_to(items, DOWN)
        mask.shift(DOWN)
        self.play(Write(mask))

        brace = Brace(mask, UP)
        self.saveObject(brace)
        braceText = brace.get_text('n')
        self.saveObject(braceText)
        self.play(GrowFromCenter(brace), Write(braceText))

        dp = TextMobject('dp[mask]')
        self.saveObject(dp)
        dp.next_to(mask, DOWN)
        dp.shift(DOWN * 0.8)
        self.play(Write(dp))
        
        leftArrow = Arrow(DOWN * 1, DOWN * 2.2 + LEFT * 4)
        self.saveObject(leftArrow)
        boxMask = TextMobject(r'boxMask\\(предметы из текущей коробки)')
        boxMask.scale(0.8)
        self.saveObject(boxMask)
        boxMask.next_to(leftArrow, DOWN)
        boxMask.shift(LEFT * 1.5)
        self.play(GrowArrow(leftArrow), Write(boxMask))

        rightArrow = Arrow(DOWN * 1, DOWN * 2.2 + RIGHT * 4)
        self.saveObject(rightArrow)
        otherMask = TextMobject(r'otherMask\\(все остальное)')
        otherMask.scale(0.8)
        self.saveObject(otherMask)
        otherMask.next_to(rightArrow, DOWN)
        otherMask.shift(RIGHT * 1.5)
        self.play(GrowArrow(rightArrow), Write(otherMask))

    def timeComplexity(self):
        formula = TexMobject(r'dp[mask] = \min_{boxMask} dp[otherMask] + cost(boxMask)')
        self.saveObject(formula)
        formula.shift(UP)
        self.play(Write(formula))

        time = TexMobject(r'Time = \mathbb{O}\left(3^n \cdot k\right)')
        self.saveObject(time)
        time.shift(DOWN)
        self.play(Write(time))


class RealisationScene(MyScene):

    def construct(self):
        self.makeScheme()
        self.wait(1)
        self.wanish()

    def makeScheme(self):
        clientRect = Rectangle(height=3.0, width=5.0)
        self.saveObject(clientRect)
        clientRect.to_edge(LEFT)
        clientRect.shift(DOWN)

        clientText = TextMobject(r'Клиент\\Графическое приложение\\C++\\SFML\\Логирование: easylogging')
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

        self.play(FadeIn(dbRect), Write(dbText), GrowArrow(shopDBArrow))
