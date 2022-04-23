import matplotlib.pyplot as plt
import base64
from io import BytesIO

languages = ["fortran","cpp"]
test_cases = ["poisson2d"]

benchmark_results = {}
for case in test_cases:
    benchmark_results[case] = []

for l in languages:
    with open("{}_results.txt".format(l)) as f:
        lines = f.readlines()
        for i in range(len(lines)):
            tokens = lines[i].split(',')
            benchmark_results[tokens[1].strip()].append(float(tokens[2]))


fig, ax = plt.subplots()

for case, results in benchmark_results.items():
    ax.scatter(list(range(len(languages))), results,label=case)

ax.set_xticks(list(range(len(languages))), labels=languages)
ax.legend()

tmpfile = BytesIO()
fig.savefig(tmpfile, format='png')
encoded = base64.b64encode(tmpfile.getvalue()).decode('utf-8')

html_pre = '''
<html>
<body>
<h2>Benchmark results</h2>
'''
html_post = '''
</body>
</html>
'''

html = html_pre + '<img src=\'data:image/png;base64,{}\'>'.format(encoded) + html_post

with open('index.html','w') as f:
    f.write(html)
