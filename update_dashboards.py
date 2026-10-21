import os
import re

def update_file(path, is_doctor):
    if not os.path.exists(path):
        return
    with open(path, 'r', encoding='utf-8') as f:
        content = f.read()

    # 1. Update zoneColor to accept (p, t) and return gradient
    content = re.sub(
        r'function zoneColor\(v\)\{.*?\}',
        r'''function zoneColor(p, t){
  const pScore = Math.min(100, (p / 90) * 100);
  const tScore = Math.min(100, Math.max(0, (t - 32) / 3 * 100));
  const combined = (pScore * 0.6) + (tScore * 0.4);
  const hue = Math.max(0, 120 - (combined * 1.2)); 
  return hsl(, 85%, 50%);
}''',
        content,
        flags=re.DOTALL
    )

    # 2. Update footSvg to pass (val.p, val.t)
    content = re.sub(
        r'const c = zoneColor\(v\);',
        r'const c = zoneColor(v.p, v.t);',
        content
    )
    content = re.sub(
        r'const glowR = z\.r \* \(1\.7 \+ v/60\);',
        r'const glowR = z.r * (1.7 + v.p/60);',
        content
    )
    content = re.sub(
        r'stop-opacity="\$\{Math\.min\(0\.85, 0\.28 \+ v/130\)\}"',
        r'stop-opacity=""',
        content
    )
    content = re.sub(
        r'stop-opacity="\$\{Math\.min\(0\.45, 0\.12 \+ v/220\)\}"',
        r'stop-opacity=""',
        content
    )

    # 3. Update computeHeatmapZones to return {p, t} objects
    # This might be tricky with regex, we can replace the whole function
    compute_zones = '''function computeHeatmapZones(risk, patient){
  const escalated = patient.escalate && risk.category !== 'calibrating';
  const baseT = patient.baseline ? patient.baseline.temp : 32.0;
  if(!escalated){
    return {
      left: BASE_ZONE_LOAD.map(v=>({ p: Math.max(4, v+rand(-4,4)), t: baseT + rand(-0.2,0.2) })),
      right: BASE_ZONE_LOAD.map(v=>({ p: Math.max(4, v+rand(-4,4)), t: baseT + rand(-0.2,0.2) }))
    };
  }
  const left = BASE_ZONE_LOAD.map((v,i)=>{
    if(i===6 || i===7) return { p: Math.min(97, v + risk.score*0.9 + rand(-2,2)), t: baseT + (risk.score/100)*2.5 + rand(-0.2,0.2) };
    if(i===2 || i===3 || i===4) return { p: Math.min(72, v + risk.score*0.28 + rand(-2,2)), t: baseT + (risk.score/100)*1.0 + rand(-0.2,0.2) };
    return { p: Math.max(4, v + rand(-3,3)), t: baseT + rand(-0.2,0.2) };
  });
  const right = BASE_ZONE_LOAD.map(v=>({ p: Math.max(4, v+rand(-4,4)), t: baseT + rand(-0.2,0.2) }));
  return {left, right};
}'''
    content = re.sub(r'function computeHeatmapZones\(risk, patient\)\{.*?(?=function|\/\* =====================)', compute_zones + '\n\n', content, flags=re.DOTALL)

    if is_doctor:
        # Remove Humidity UI
        content = re.sub(r'<div class="sensor-tile"><div class="s-lbl">Humidity.*?</div></div>', '', content)
        content = re.sub(r'document\.getElementById\(\'sv-humidity\'\)\.innerHTML.*?;', '', content)
        content = re.sub(r'Humidity: \$\{Math\.round\(latest\.humidity\)\} %', '', content)
        content = re.sub(r'Humidity:\[risk\.factors\.humidity,5\],?\s*', '', content)

        # Remove AI Prediction UI
        content = re.sub(r'<div class="nav-item"[^>]*data-view="ai".*?</div>', '', content)
        content = re.sub(r'<!-- AI PREDICTION -->.*?<div class="view" id="view-heatmap">', '<div class="view" id="view-heatmap">', content, flags=re.DOTALL)
        content = re.sub(r'<div class="glass-card ai-panel">.*?<!-- MAIN DASHBOARD -->', '<!-- MAIN DASHBOARD -->', content, flags=re.DOTALL)
        content = re.sub(r'<div class="glass-card stat-card"><div class="stat-top"><div class="stat-icon"[^>]*>??</div>.*?</div>', '', content)

    with open(path, 'w', encoding='utf-8') as f:
        f.write(content)

update_file(r'c:\Users\Srineedhi\Desktop\Diab-Aid\patient-dashboard.html', False)
update_file(r'c:\Users\Srineedhi\Desktop\Diab-Aid\footguard-dashboard.html', True)
print("Updated successfully.")
