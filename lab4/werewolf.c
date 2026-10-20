#include <stdio.h>
#include <stdlib.h>

#define CAP 105

enum {
    UNKNOWN = -1,
    HUMAN = 0,
    WOLF = 1
};

int n, m, l;
int statement[CAP];
int identity[CAP];

/*
 * 被指认者身份已确定时，判断第 speaker 人是否撒谎。
 * 正数表示声称对方是人，负数表示声称对方是狼。
 */
int is_lying(int speaker)
{
    int target = abs(statement[speaker]);
    int claimed = statement[speaker] > 0 ? HUMAN : WOLF;

    return identity[target] != claimed;
}

/*
 * 检查当前已经确定的信息是否还可能构成合法解。
 * complete 为 1 时，所有人的身份都已确定。
 */
int feasible(int complete)
{
    int confirmed_liars = 0;
    int unresolved = 0;
    int lying_wolves = 0;

    for (int speaker = 1; speaker <= n; ++speaker) {
        int target = abs(statement[speaker]);

        if (identity[target] == UNKNOWN) {
            ++unresolved;
            continue;
        }

        if (is_lying(speaker)) {
            ++confirmed_liars;

            if (identity[speaker] == WOLF)
                ++lying_wolves;
        }
    }

    /* 已经确定的谎言太多。 */
    if (confirmed_liars > l)
        return 0;

    /* 即使未确定的陈述全部为假，也凑不够。 */
    if (confirmed_liars + unresolved < l)
        return 0;

    /* 已经确定有 m 个狼人撒谎，无法保留说真话的狼人。 */
    if (lying_wolves >= m)
        return 0;

    if (complete) {
        return confirmed_liars == l
            && lying_wolves >= 1
            && lying_wolves < m;
    }

    /* TODO：可以在这里添加自己推导出的更强剪枝。 */

    return 1;
}

/*
 * 还需决定编号 1～next 的身份。
 * 编号 next+1～n 的身份已经确定。
 */
int search(int next, int wolf_count)
{
    if (wolf_count > m)
        return 0;

    if (wolf_count + next < m)
        return 0;

    if (!feasible(next == 0))
        return 0;

    if (next == 0)
        return 1;

    /* 大编号优先成为狼人，使答案按字典序从大到小搜索。 */
    identity[next] = WOLF;
    if (search(next - 1, wolf_count + 1))
        return 1;

    identity[next] = HUMAN;
    if (search(next - 1, wolf_count))
        return 1;

    /* 两个分支都失败，撤销本层选择。 */
    identity[next] = UNKNOWN;
    return 0;
}

int main(void)
{
    if (scanf("%d%d%d", &n, &m, &l) != 3)
        return 0;

    for (int i = 1; i <= n; ++i) {
        scanf("%d", &statement[i]);
        identity[i] = UNKNOWN;
    }

    if (!search(n, 0)) {
        puts("No Solution");
        return 0;
    }

    int first = 1;

    for (int i = n; i >= 1; --i) {
        if (identity[i] == WOLF) {
            if (!first)
                putchar(' ');

            printf("%d", i);
            first = 0;
        }
    }

    putchar('\n');
    return 0;
}