// Lil Terminal
#include "lil.h"
#include "dom.h"
#include <sys/wait.h>
#include <unistd.h>

lv*n_exit(lv*self,lv*a){(void)self;exit(ln(l_first(a)));}
lv*n_input(lv*self,lv*a){
	(void)self;n_printf(a,0,stdout);fflush(stdout);str r=str_new();char b[1];
	while(read(0,b,sizeof(b))>0&&b[0]!='\n')str_addc(&r,b[0]);return lmstr(r);
}
lv*n_dir(lv*self,lv*a){(void)self;lv*r=directory_enumerate(ls(l_first(a))->sv,filter_none,1);r->kv[0]=lmistr("dir");return r;}
lv*n_path(lv*self,lv*a){
	(void)self;lv*x=a->c>0?ls(a->lv[0]):lms(0),*y=a->c>1?ls(a->lv[1]):lms(0);str t=str_new();char out[4096];
	str_add(&t,x->sv,x->c);if(y->c)str_addc(&t,'/');str_add(&t,y->sv,y->c),str_term(&t);
	char*e=realpath(t.sv,out);free(t.sv);return e!=NULL?lmcstr(out):lmistr("");
}
lv*n_readwav(lv*self,lv*a){
	(void)self;lv*name=ls(l_first(a));int offset=a->c>1?MAX(0,ln(a->lv[1])):0, size=0;
	struct stat st;if(stat(name->sv,&st)||st.st_size<13)return sound_make(lms(size));
	char*data=calloc(size=st.st_size,1);FILE*f=fopen(name->sv,"rb");fread(data,1,st.st_size,f),fclose(f);
	char HEAD[]={'R','I','F','F',0xFF,0xFF,0xFF,0xFF,'W','A','V','E','f','m','t',' ',16,0,0,0,1,0,1,0,64,31,0,0,64,31,0,0,1,0,8,0,'d','a','t','a'};
	for(int z=0;z<40&&z<size;z++)if(0xFF!=(0xFF&HEAD[z])&&HEAD[z]!=data[z])return free(data),sound_make(lms(0));
	int samples=((0xFF&data[43])<<24)|((0xFF&data[42])<<16)|((0xFF&data[41])<<8)|(0xFF&data[40]);
	lv*r=lms(CLAMP(0,samples-offset,10*SFX_RATE));EACH(z,r)r->sv[z]=0xFF&(data[44+z+offset]-128);
	return free(data),sound_make(r);
}
lv*n_readfile(lv*self,lv*a){
	lv*name=ls(l_first(a));
	if(a->c>1&&matchr(lmistr("array"),a->lv[1]))return readbin(name);
	if(has_suffix(name->sv,".gif"))return n_readgif(self,a);
	if(has_suffix(name->sv,".wav"))return n_readwav(self,a);
	return n_read(self,a);
}
lv*n_writefile(lv*self,lv*a){
	lv*value=a->c>1?a->lv[1]:lms(0);
	if(array_is(value))return writebin(l_first(a),value);
	if(sound_is(value))return n_writewav(self,a);
	if(image_is(value))return n_writegif(self,a);
	if(lil(value)){EACH(z,value)if(image_is(value->lv[z]))return n_writegif(self,a);}
	return n_write(self,a);
}
lv*n_readdeck(lv*self,lv*a){return deck_read(lin(l_first(a))?lmistr(""):n_read(self,a));}
lv*n_writedeck(lv*self,lv*a){
	lv*path=ls(l_first(a));int html=0;if(has_suffix(path->sv,".html")){html=1;}
	lv*v=deck_write(a->c<2?NONE:a->lv[1],html);if(v->c<1)return NONE;return n_write(self,lml2(path,v));
}
lv*n_shell(lv*self,lv*a){
	(void)self;lv*x=ls(l_first(a)),*r=lmd();FILE*child=popen(x->sv,"r");str o=str_new();
	while(1){int c=fgetc(child);if(feof(child))break;str_addc(&o,c);}int e=pclose(child);
	return dset(r,lmistr("out"),lmstr(o)),dset(r,lmistr("exit"),lmn(WIFEXITED(e)?WEXITSTATUS(e): -1)),r;
}
lv*runstring(char*t,lv*env){
	lv* prog=parse(t);if(perr())return fprintf(stderr,"(%d:%d) %s\n",par.r+1,par.c+1,par.error),NONE;
	return run(prog,env);
}
lv*runfile(char*path,lv*env){
	struct stat st;if(stat(path,&st)){fprintf(stderr,"unable to open '%s'\n",path);return NONE;}
	return runstring(n_read(NULL,l_list(lmcstr(path)))->sv,env);
}
extern char **environ;

void go_notify(lv*deck,lv*args,int dest){(void)deck,(void)args,(void)dest;}
lv* n_print(lv*self,lv*a){(void)self;return n_printf(a,1,stdout);}
lv* n_alert(lv*self,lv*z){(void)self,(void)z;return ONE;}
lv* n_open (lv*self,lv*z){(void)self,(void)z;return lmistr("");}
lv* n_save (lv*self,lv*z){(void)self,(void)z;return NONE;}
lv* n_play(lv*self,lv*z){
	lv*x=l_first(z);char*n=ls(dget(self->b,lmistr("title")))->sv;
	if(sound_is(z)){printf("deck %s played sound.",n);}
	else{printf("deck %s played sound '%s'.",n,ls(x)->sv);}
	return x;
}
lv* n_show(lv*self,lv*a){
	(void)self;str s=str_new();EACH(z,a){if(z)str_addc(&s,' ');show(&s,a->lv[z],a->c==1);}
	printf("%s\n",lmstr(s)->sv);return l_first(a);
}

// Entrypoint

int main(int argc,char**argv){
	init_interns();
	state.external=1;
	lv* env=lmenv(NULL);
	dset(env,lmistr("show"     ),lmnat(n_show,NULL));
	dset(env,lmistr("print"    ),lmnat(n_print,NULL));
	dset(env,lmistr("error"    ),lmnat(n_error,NULL));
	dset(env,lmistr("input"    ),lmnat(n_input,NULL));
	dset(env,lmistr("dir"      ),lmnat(n_dir,NULL));
	dset(env,lmistr("read"     ),lmnat(n_readfile,NULL));
	dset(env,lmistr("write"    ),lmnat(n_writefile,NULL));
	dset(env,lmistr("path"     ),lmnat(n_path,NULL));
	dset(env,lmistr("exit"     ),lmnat(n_exit,NULL));
	dset(env,lmistr("shell"    ),lmnat(n_shell,NULL));
	dset(env,lmistr("eval"     ),lmnat(n_eval,NULL));
	dset(env,lmistr("random"   ),lmnat(n_random,NULL));
	dset(env,lmistr("array"    ),lmnat(n_array,NULL));
	dset(env,lmistr("image"    ),lmnat(n_image,NULL));
	dset(env,lmistr("sound"    ),lmnat(n_sound,NULL));
	dset(env,lmistr("readcsv"  ),lmnat(n_readcsv,NULL));
	dset(env,lmistr("writecsv" ),lmnat(n_writecsv,NULL));
	dset(env,lmistr("readxml"  ),lmnat(n_readxml,NULL));
	dset(env,lmistr("writexml" ),lmnat(n_writexml,NULL));
	dset(env,lmistr("readdeck" ),lmnat(n_readdeck,NULL));
	dset(env,lmistr("writedeck"),lmnat(n_writedeck,NULL));
	constants(env);
	lv* a=lml(argc);for(int z=0;z<argc;z++)a->lv[z]=lmcstr(argv[z]);
	dset(env,lmistr("args"),a);
	lv* e=lmd();for(int z=0;environ[z];z++){
		int i=0;while(environ[z][i]&&environ[z][i]!='=')i++;
		lv*k=lms(i);memcpy(k->sv,environ[z],i),dset(e,k,lmcstr(environ[z]+i+1));
	}dset(env,lmistr("env"),e);
	char*home=getenv("LIL_HOME");if(home){
		struct dirent*find;DIR*dir=opendir(home);if(dir){while((find=readdir(dir))){
			char path[4096];snprintf(path,sizeof(path),"%s/%s",home,find->d_name);
			if(has_suffix(path,".lil"))runfile(path,env);
		}}closedir(dir);
	}
	int repl=1;for(int z=1;z<argc;z++){
		if(!strcmp(argv[z],"-h")){repl=0;
			printf("usage: %s [FILE.lil...] [-e EXPR...]\nif present, execute a FILE and exit\n",argv[0]);
			printf("-e : evaluate STRING and exit\n-h : display this information\n");
		}
		else if(!strcmp(argv[z],"-e")){repl=0;
			if(z+1>=argc)fprintf(stderr,"no expression specified.\n"),exit(1);
			runstring(argv[z+1],env),z++;
		}
		else if(has_suffix(argv[z],".lil")){repl=0;runfile(argv[z],env),z++;}
	}if(!repl){exit(0);}
	str r=str_new();while(1){
		printf("  ");fflush(stdout);
		r.c=0;char b[1];while(read(0,b,sizeof(b))>0&&b[0]!='\n')str_addc(&r,b[0]);
		str_term(&r);lv*prog=parse(r.sv);
		if(perr()){for(int z=0;z<par.c+2;z++)printf(" ");printf("^\n%s\n",par.error);}
		else{lv*x=run(prog,env);dset(env,lmistr("_"),x);debug_show(x);}
	}
}
