import matplotlib.pyplot as plt
import base64
from io import BytesIO
import sys

languages = ["fortran","cpp","rust"]
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
ax.set_ylabel("Duration (s)")
ax.legend()

tmpfile = BytesIO()
fig.savefig(tmpfile, format='png')
encoded = base64.b64encode(tmpfile.getvalue()).decode('utf-8')

html_template = '''<html>
<head>
<style type="text/css">
.mainbody {{
    max-width: 1000px;
    margin-right: auto;
    margin-left: auto;
}}
</style>
</head>
<body>
<div class="mainbody">
<h1>Benchmark results</h1>
{img}
<ul>
<li>Commit: {sha}</li>
<li>OS: {OS}</li>
<li>CPU: {CPU}</li>
</ul>
<p>Repository: <a href="https://github.com/St-Maxwell/benchmark">https://github.com/St-Maxwell/benchmark</a></p>
</div>
</body>
</html>'''

sha = sys.argv[1]
OS = sys.argv[2]
CPU = sys.argv[3]
html = html_template.format(img='<img src=\'data:image/png;base64,{}\'>'.format(encoded),
                            sha=sha, OS=OS, CPU=CPU)

with open('index.html','w') as f:
    f.write(html)
