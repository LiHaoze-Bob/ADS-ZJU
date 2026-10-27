#include <stdio.h>
#include <stdlib.h>

double c, d, d_avg;
int n;

typedef struct station {
    double price;
    double distance;
} station;

int compare(const void *a, const void *b) {
    const station *x = a;
    const station *y = b;
    if (x->distance != y->distance) {
        return (x->distance > y->distance) ? 1 : -1;
    }
    return (x->price > y->price) - (x->price < y->price);
}

int main(void) {
    if (scanf("%lf %lf %lf %d", &c, &d, &d_avg, &n) != 4) {
        return 0;
    }
    station *p = malloc(sizeof(*p) * (n + 1));
    if (p == NULL) {
        return 1;
    }
    for (int i = 0; i < n; i++) {
        if (scanf("%lf %lf", &p[i].price, &p[i].distance) != 2) {
            free(p);
            return 1;
        }
    }
    qsort(p, n, sizeof(*p), compare);

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (count == 0 || p[i].distance != p[count - 1].distance) {
            p[count++] = p[i];
        }
    }
    n = count;
    p[n++] = (station){0, d};

    if (p[0].distance != 0) {
        printf("The maximum travel distance = 0.00\n");
        free(p);
        return 0;
    }

    double range = c * d_avg;
    for (int i = 0; i + 1 < n; i++) {
        if (p[i + 1].distance - p[i].distance > range + 1e-8) {
            printf("The maximum travel distance = %.2f\n", p[i].distance + range);
            free(p);
            return 0;
        }
    }

    double cur_gas = 0;
    double cost = 0;
    for (int i = 0; i + 1 < n; i++) {
        double goal = c;
        double remaining = (d - p[i].distance) / d_avg;
        if (remaining < goal) {
            goal = remaining;
        }
        for (int j = i + 1; j < n; j++) {
            if (p[j].distance - p[i].distance > range + 1e-8) {
                break;
            }
            if (p[j].price < p[i].price) {
                goal = (p[j].distance - p[i].distance) / d_avg;
                break;
            }
        }
        if (goal > cur_gas) {
            cost += (goal - cur_gas) * p[i].price;
            cur_gas = goal;
        }
        cur_gas -= (p[i + 1].distance - p[i].distance) / d_avg;
        if (cur_gas < 0) {
            cur_gas = 0;
        }
    }

    printf("%.2f\n", cost);
    free(p);
    return 0;
}
