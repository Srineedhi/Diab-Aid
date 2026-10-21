import os
import sys

# reuse the same script logic
with open(r'c:\Users\Srineedhi\Desktop\Diab-Aid\update_dashboards.py', 'r', encoding='utf-8') as f:
    code = f.read()

code = code.replace(r'update_file(r''c:\Users\Srineedhi\Desktop\Diab-Aid\footguard-dashboard.html'', True)', r'update_file(r''c:\Users\Srineedhi\Desktop\Diab-Aid\SC\footguard-dashboard.html'', True)')
code = code.replace(r'update_file(r''c:\Users\Srineedhi\Desktop\Diab-Aid\patient-dashboard.html'', False)', r'update_file(r''c:\Users\Srineedhi\Desktop\Diab-Aid\SC\patient-dashboard.html'', False)')

with open(r'c:\Users\Srineedhi\Desktop\Diab-Aid\update_sc_dashboards.py', 'w', encoding='utf-8') as f:
    f.write(code)

