
function select_all(frm)
{
  var i,slength=0;
  if (frm.sel_id==null) { return false; }
  var sall=frm.sel_all.checked;
  if (frm.sel_id.length)
  {
    slength=frm.sel_id.length;
    for (i=0;i<slength;i++) { frm.sel_id[i].checked=sall; }
  }
  else
  { frm.sel_id.checked=sall; }
}

function sel_click(frm)
{
  var i,slength=0,issel=false;
  if (frm.sel_id!=null)
  {
    if (frm.sel_id.length)
    {
      slength=frm.sel_id.length;
      for (i=0;i<slength;i++) { if (frm.sel_id[i].checked==true) { issel=true; break; } }
    }
    else
    { if (frm.sel_id.checked==true) { issel=true; } }
  }
  if (issel==true)
  {
    var cf=window.confirm("ִ�д˲���������޷��ָ�����ȷ����");
    if (cf)
    { return true; }
    else
    { return false; }
  }
  else
  {
    alert("û��ѡ���κμ�¼��");
    return false;
  }
}
