# coding:utf-8=True
import math

#
# 每月还款 = P * (R * (1 + R)^N)/((1 + R)^N - 1)
#
# P ：当前剩余本金
# N ：总还款月数
# R ：月利率
#
def pmt(amount, months, interest):
    temp = math.pow(interest + 1, months)
    return round(interest * temp / (temp - 1) * amount, 2)


def interests_count(money, months, interest, months_return=[], moneys_return=[]):
    money_last = money
    interest_month = interest / 12
    money_month = pmt(money_last, months, interest_month)
    print '每月(1 - )：', money_month
    # 本金
    principals = []
    # 利息
    interests = []

    for month in range(0, months):
        if len(months_return) > 0 and month in months_return:
            for m in months_return:
                if m == month:
                    money_last -= moneys_return[months_return.index(m)]
                    break
            money_month = pmt(money_last, months - month, interest_month)
            print '每月({} - )：{} (剩余本金：{})'.format(month + 1, money_month, money_last)
        interests.insert(month, round(money_last * interest_month, 2))
        principals.insert(month, round(money_month - interests[month], 2))
        money_last -= principals[month]

#    print principals
#    print interests
    if len(months_return) > 0:
        print '总本金 :', sum(principals) + sum(money_return)
    else:
        print '总本金 :', sum(principals)
    print '总利息 :', sum(interests)
    return sum(interests)


if __name__ == '__main__':
    month_return = [24, 36, 48, 60]
    money_return = [100000, 100000, 100000, 100000]
    print '不提前还款'
    interests_count(1330000, 360, 0.049 * 1.15)
    print '1年还10w'
    interests_count(1330000, 360, 0.049 * 1.15, month_return, money_return)
