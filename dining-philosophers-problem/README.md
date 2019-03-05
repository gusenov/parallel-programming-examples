# Case 1

```text
     f3
p4   ||   p3
  f4 || f2
p5   ||   p2
  f5 || f1
     p1
```

Forks:

- left fork = p - 1 != 0 ? p - 1 : 5
- right fork = p

| Philosopher | Forks   | Lock    | Unlock |
| ----------- | ------- | ------- | ------ |
| 1           | 5 and 1 | 2 and 5 | 3 or 4 |
| 2           | 1 and 2 | 1 and 3 | 4 or 5 |
| 3           | 2 and 3 | 2 and 4 | 1 or 5 |
| 4           | 3 and 4 | 3 and 5 | 1 or 2 |
| 5           | 4 and 5 | 1 and 4 | 2 or 3 |

# Case 2

```text
     f4
p4   ||   p3
  f5 || f3
p5   ||   p2
  f1 || f2
     p1
```

Forks:

- left fork = p
- right fork = p + 1 != 5 ? (p + 1) mod 5 : 5

| Philosopher | Forks   |
| ----------- | ------- |
| 1           | 1 and 2 |
| 2           | 2 and 3 |
| 3           | 4 and 4 |
| 4           | 4 and 5 |
| 5           | 5 and 1 |

# Screenshots

![Снимок экрана от 2019-03-05 22-09-23](screenshots/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%20%D0%BE%D1%82%202019-03-05%2022-09-23.png)

![Снимок экрана от 2019-03-05 22-03-54](screenshots/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%20%D0%BE%D1%82%202019-03-05%2022-03-54.png)

![Снимок экрана от 2019-03-05 22-08-53](screenshots/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%20%D0%BE%D1%82%202019-03-05%2022-08-53.png)

![Снимок экрана от 2019-03-05 22-02-12](screenshots/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%20%D0%BE%D1%82%202019-03-05%2022-02-12.png)

![Снимок экрана от 2019-03-05 22-11-14](screenshots/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%20%D0%BE%D1%82%202019-03-05%2022-11-14.png)

![Снимок экрана от 2019-03-05 22-07-54](screenshots/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%20%D0%BE%D1%82%202019-03-05%2022-07-54.png)

