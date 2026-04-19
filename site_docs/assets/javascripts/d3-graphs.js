(function () {
  function safeParse(jsonText) {
    try {
      return JSON.parse(jsonText);
    } catch (error) {
      console.warn("Falha ao interpretar dados D3.", error);
      return null;
    }
  }

  function getStatusColor(status) {
    const palette = {
      Concluido: "#0f766e",
      "Em andamento": "#f59e0b",
      Pendente: "#dc2626",
    };
    return palette[status] || "#64748b";
  }

  function clearNode(node) {
    while (node.firstChild) {
      node.removeChild(node.firstChild);
    }
  }

  function renderStatusDonut(container) {
    const config = safeParse(container.dataset.chart || "{}");
    if (!config || !Array.isArray(config.items) || !window.d3) {
      return;
    }

    clearNode(container);

    const width = 340;
    const height = 280;
    const radius = Math.min(width, height) / 2 - 18;
    const total = d3.sum(config.items, (item) => item.value);

    const svg = d3
      .select(container)
      .append("svg")
      .attr("viewBox", `0 0 ${width} ${height + 70}`)
      .attr("class", "d3-svg");

    const chart = svg
      .append("g")
      .attr("transform", `translate(${width / 2}, ${height / 2})`);

    const pie = d3
      .pie()
      .sort(null)
      .value((item) => item.value);

    const arc = d3.arc().innerRadius(radius * 0.58).outerRadius(radius);

    chart
      .selectAll("path")
      .data(pie(config.items))
      .enter()
      .append("path")
      .attr("d", arc)
      .attr("fill", (d) => d.data.color || getStatusColor(d.data.label))
      .attr("stroke", "rgba(255,255,255,0.92)")
      .attr("stroke-width", 3);

    chart
      .append("text")
      .attr("class", "d3-center-value")
      .attr("text-anchor", "middle")
      .attr("y", -4)
      .text(total);

    chart
      .append("text")
      .attr("class", "d3-center-label")
      .attr("text-anchor", "middle")
      .attr("y", 18)
      .text("frentes");

    const legend = svg.append("g").attr("transform", `translate(18, ${height + 18})`);

    config.items.forEach((item, index) => {
      const row = legend.append("g").attr("transform", `translate(${index * 106}, 0)`);
      row
        .append("rect")
        .attr("width", 12)
        .attr("height", 12)
        .attr("rx", 3)
        .attr("fill", item.color || getStatusColor(item.label));

      row
        .append("text")
        .attr("x", 18)
        .attr("y", 10)
        .attr("class", "d3-legend-text")
        .text(`${item.label}: ${item.value}`);
    });
  }

  function renderMaturityBars(container) {
    const config = safeParse(container.dataset.chart || "{}");
    if (!config || !Array.isArray(config.items) || !window.d3) {
      return;
    }

    clearNode(container);

    const margin = { top: 10, right: 18, bottom: 14, left: 160 };
    const rowHeight = 34;
    const width = 720;
    const height = margin.top + margin.bottom + config.items.length * rowHeight;

    const svg = d3
      .select(container)
      .append("svg")
      .attr("viewBox", `0 0 ${width} ${height}`)
      .attr("class", "d3-svg");

    const x = d3.scaleLinear().domain([0, 100]).range([margin.left, width - margin.right]);
    const y = d3
      .scaleBand()
      .domain(config.items.map((item) => item.label))
      .range([margin.top, height - margin.bottom])
      .padding(0.28);

    svg
      .append("g")
      .attr("class", "d3-axis d3-axis--x")
      .attr("transform", `translate(0, ${height - margin.bottom})`)
      .call(d3.axisBottom(x).ticks(5).tickFormat((value) => `${value}%`))
      .call((g) => g.select(".domain").remove());

    svg
      .append("g")
      .attr("class", "d3-axis d3-axis--y")
      .attr("transform", `translate(${margin.left}, 0)`)
      .call(d3.axisLeft(y).tickSize(0))
      .call((g) => g.select(".domain").remove());

    svg
      .append("g")
      .selectAll("rect")
      .data(config.items)
      .enter()
      .append("rect")
      .attr("x", x(0))
      .attr("y", (item) => y(item.label))
      .attr("width", (item) => x(item.score) - x(0))
      .attr("height", y.bandwidth())
      .attr("rx", 8)
      .attr("fill", (item) => getStatusColor(item.status));

    svg
      .append("g")
      .selectAll("text")
      .data(config.items)
      .enter()
      .append("text")
      .attr("class", "d3-bar-label")
      .attr("x", (item) => x(item.score) + 10)
      .attr("y", (item) => y(item.label) + y.bandwidth() / 2 + 4)
      .text((item) => `${item.score}%`);
  }

  function renderModulesTree(container) {
    const treeData = safeParse(container.dataset.tree || "{}");
    if (!treeData || !window.d3) {
      return;
    }

    clearNode(container);

    const width = Math.max(container.clientWidth || 760, 760);
    const margin = { top: 18, right: 180, bottom: 18, left: 28 };
    const root = d3.hierarchy(treeData);
    root.dx = 34;
    root.dy = Math.max(140, (width - margin.left - margin.right) / (root.height + 1));

    d3.tree().nodeSize([root.dx, root.dy])(root);

    let x0 = Infinity;
    let x1 = -x0;
    root.each((node) => {
      if (node.x > x1) x1 = node.x;
      if (node.x < x0) x0 = node.x;
    });

    const height = x1 - x0 + margin.top + margin.bottom;

    const svg = d3
      .select(container)
      .append("svg")
      .attr("viewBox", `0 0 ${width} ${height}`)
      .attr("class", "d3-svg");

    const graph = svg.append("g").attr("transform", `translate(${margin.left}, ${margin.top - x0})`);

    graph
      .append("g")
      .selectAll("path")
      .data(root.links())
      .enter()
      .append("path")
      .attr("class", "d3-tree-link")
      .attr(
        "d",
        d3
          .linkHorizontal()
          .x((node) => node.y)
          .y((node) => node.x)
      );

    const node = graph
      .append("g")
      .selectAll("g")
      .data(root.descendants())
      .enter()
      .append("g")
      .attr("transform", (d) => `translate(${d.y},${d.x})`);

    node
      .append("circle")
      .attr("r", (d) => (d.depth === 0 ? 9 : 6))
      .attr("fill", (d) => {
        if (d.depth === 0) return "#0f766e";
        if (d.depth === 1) return "#f59e0b";
        return "#155e75";
      })
      .attr("class", "d3-tree-node");

    node
      .append("text")
      .attr("dy", "0.32em")
      .attr("x", (d) => (d.children ? -12 : 12))
      .attr("text-anchor", (d) => (d.children ? "end" : "start"))
      .attr("class", "d3-tree-label")
      .text((d) => d.data.name);
  }

  function renderAllD3Blocks() {
    if (!window.d3) {
      return;
    }

    document.querySelectorAll(".d3-status-donut").forEach(renderStatusDonut);
    document.querySelectorAll(".d3-maturity-bars").forEach(renderMaturityBars);
    document.querySelectorAll(".d3-modules-tree").forEach(renderModulesTree);
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", renderAllD3Blocks);
  } else {
    renderAllD3Blocks();
  }

  document.addEventListener("DOMContentLoaded", renderAllD3Blocks);
  document.addEventListener("mkdocs:searchReady", renderAllD3Blocks);
  document.addEventListener("mkdocs:pageLoaded", renderAllD3Blocks);
})();
