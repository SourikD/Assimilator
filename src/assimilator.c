#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#define ORI 111114105
#define SLT 115108116
#define BEQ 98101113
#define LUI  108117105
#define LW  108119
#define ADDU 97100100117
#define ADDIU 97100100105117
#define BNE 98110101
#define SW  115119
#define J 106
#define JR 106114
#define JAL 106097108
#define JALR 106097108114
#define BLEZ 98108101122
#define BGTZ 98103116122
#define BLTZ 98108116122
#define BGEZ 98103101122
#define SUB 115117098
#define SUBU  115117098117
#define ANDI  97110100105
#define AND 97110100
#define OR 111114
#define XOR 120111114
#define XORI 120111114105
#define NOR  110111114
#define SLL 115108108
#define SRL 115114108
#define SRA  1151144097
#define ADDI 111114105

long  dec(char[]);
long  chartoint(int);
int load_assembly(char* file_name);
char* get_substr(char* ,int ,char,char*);
int decode_execute(int,char* ,int,int ,int ,int ,long,long,int);
int size_imm;
int indx;	
struct memory
		
		{
			
			char op_str[10];
			long op_dec;
			int immediate;
			int dest_id;
			int  src1_id;
			int src2_id;
			long  data;
			int imm_valid;	
		};
			
struct memory memblock[2048];
int pc,npc,last_index;
int regfile[34];	
int main(int argc,char* argv[])
{
	
	last_index = load_assembly(argv[1]);
	
	indx=0;
	pc=0,npc=0;
	FILE *f_reg;
	FILE *f_mem;
	FILE *f_inst;
	int j=0;
	regfile[0]=0;
	int i;
	for(i=0;i<34;i++)
		regfile[i] = 0;
	f_inst=fopen("instruction.txt","w");


	while(indx<last_index)
	{
			
		indx=pc/4;
		
		npc = decode_execute(pc,memblock[indx].op_str,memblock[indx].dest_id,
				memblock[indx].src1_id,memblock[indx].src2_id,
				memblock[indx].immediate,memblock[indx].data,
				memblock[indx].op_dec,memblock[indx].imm_valid);//prints   lines in the memory one by one from i=0

		/*fprintf(stdout,"PC: %d %s %d %d %d %d\n",
			pc,memblock[indx].op_str,memblock[indx].dest_id,
			memblock[indx].src1_id,memblock[indx].src2_id,
			memblock[indx].immediate);
		
		
		fprintf(f_inst,"PC: %d %s %d %d %d %d\n",
			pc,memblock[indx].op_str,memblock[indx].dest_id,
			memblock[indx].src1_id,memblock[indx].src2_id,
			memblock[indx].immediate);
		*/
		pc = npc;
	} //while(index < last_index)

	f_reg=fopen("regfile.txt","w");//write the regfile contents in a  new file
	fprintf(f_reg,"the regfile contents in the corresponding indices  are:\n");
	for(j=0;j<34;j++)
	{
		fprintf(f_reg,"%d in indx %d\n", regfile[j],j);
	}
	fclose(f_reg);	
		
	f_mem=fopen("memory.txt","w");//write the memory array contents into  a  new file
	fprintf(f_mem,"contents of the memory are: ");
	for(j=0;j<2048;j++)
	{

		fprintf(f_mem," opstr:%s,  dest_id:%d ,src1_id:%d, src2_id: %d, immediate:%d data: %ld in index %d\n",memblock[j].op_str,memblock[j].dest_id,memblock[j].src1_id,memblock[j].src2_id,memblock[j].immediate, memblock[j].data,j);//prints the read line from the file,fetch instruction
	}
	fclose(f_mem);
		
return 0;
}	
		
int decode_execute(int cpc, char* opstring,int dest_id,int src1_id,int src2_id,int immediate,long data,long opdecimal,int imm_valid)
{
	int result=0,res=0;
//	int regfile[34];
//	regfile[0]=0;
	char execution_str[300];
	char inst_display[200];
	int npc=cpc+8;
	switch(opdecimal)
	{
		case ADDIU:
				result=regfile[src1_id]+immediate;
				int operand1=regfile[src1_id];
				regfile[dest_id]=result;
				sprintf(inst_display,"%s %d %d %d",opstring,dest_id,src1_id,immediate);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,operand1,immediate,result); 
				break;
		case ADDU:	result=regfile[src1_id]+regfile[src2_id];
				regfile[dest_id]=result;
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,src2_id);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,regfile[src1_id],immediate,regfile[dest_id]); 
			
				break;

		case ORI:	result=regfile[src1_id] | immediate;
				regfile[dest_id]=result;
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,immediate);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,regfile[src1_id],immediate,regfile[dest_id]); 

				break;

		case BEQ:	if(regfile[src1_id]==regfile[dest_id])
				{
					npc=cpc+8+immediate;
				}
				else
				{
					npc=cpc+8;
				}
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,immediate);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and npc are:%x %s %x %x %x",
					cpc,opstring,regfile[src1_id],regfile[dest_id],npc); 
				break;
				
		case BNE:	if(regfile[src1_id]!=regfile[dest_id])
				{
					npc=cpc+8+immediate;
				}
				else
					npc=cpc+8;	
				
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,immediate);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and npc are:%x %s %x %x %x",
					cpc,opstring,regfile[src1_id],regfile[dest_id],npc); 
				
				break;

		case LW:	if(!imm_valid)
				{
	
					res=regfile[src1_id]+regfile[src2_id];
					regfile[dest_id]=memblock[res/4].data;			
					sprintf(inst_display,"%s r%d r%d r%d",opstring,dest_id,src1_id,src2_id);
					sprintf(execution_str,"The PC,opcode, operand1,operand 2, address and destination register value are:%x %s %x %x %x %x\n",
						cpc,opstring,regfile[src1_id],regfile[src2_id],res,regfile[dest_id]); 
				
				}
				else
				{	
					res=immediate+regfile[src1_id];
					regfile[dest_id]=memblock[res/4].data;			
					sprintf(inst_display,"%s r%d r%d #%d",opstring,dest_id,src1_id,immediate);
					sprintf(execution_str,"The PC,opcode, operand1,operand 2 ,address and destination register value are:%x %s %x %x %x %x\n ",
						cpc,opstring,regfile[src1_id],immediate,res,regfile[dest_id]); 
				


				}
						
				break;
		
		case SW:	
						
				if(!imm_valid)
				{
					
					res=regfile[src1_id]+regfile[src2_id];
					memblock[res/4].data=regfile[dest_id];	
					sprintf(inst_display," %s r%d r%d r%d",opstring,dest_id,src1_id,src2_id);
					sprintf(execution_str,"The PC,opcode, operand1,operand 2,address and the destination are:%x %s %x %x  %x %x\n ",
						cpc,opstring,regfile[src1_id],regfile[src2_id],res,regfile[dest_id]);
				}
				else
				{	
					res=immediate+regfile[src1_id];
					memblock[res/4].data=regfile[dest_id];	
				
					sprintf(inst_display," %s r%d r%d #%d",opstring,dest_id,src1_id,immediate);
					sprintf(execution_str,"The PC,opcode, operand1,operand 2,address and the destination are:%x %s %x %x %x %x\n ",
						cpc,opstring,regfile[src1_id],regfile[src2_id],res,regfile[dest_id]);




				}
				break;

		case SLT:		
				if(regfile[src1_id]<regfile[src2_id])
				{
					regfile[dest_id]=1;
				}
				else
				{
					regfile[dest_id]=0;
				}	
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,src2_id);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,regfile[src1_id],regfile[src2_id],regfile[dest_id]); 

				break;

		case LUI:	
				immediate=immediate<<16;
				regfile[dest_id]=immediate;
				sprintf(inst_display," %s %d %d ",opstring,dest_id,immediate);
				sprintf(execution_str,"The PC,opcode, operand1 and result are:%x %s %x %x ",
					cpc,opstring,immediate,regfile[dest_id]); 
				
				break;
	
		case SUBU:	result=regfile[src1_id]-regfile[src2_id];
				regfile[dest_id]=result;
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,src2_id);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,regfile[src1_id],regfile[src2_id],regfile[dest_id]); 

				break;
		case ANDI:	result=regfile[src1_id] & immediate;
				regfile[dest_id]=result;
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,immediate);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,regfile[src2_id],immediate,regfile[dest_id]); 
				break;
	
		case AND:	result=regfile[src1_id] & regfile[src2_id];
				regfile[dest_id]=result;
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,src2_id);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,regfile[src2_id],regfile[src2_id],regfile[dest_id]); 

				break;
		case NOR: 	result=regfile[src1_id] | regfile[src2_id];
				regfile[dest_id]=~result;
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,src2_id);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,regfile[src2_id],regfile[src2_id],regfile[dest_id]); 
				break;
		case XOR:	result=regfile[src1_id] ^ regfile[src2_id];
				regfile[dest_id]=result;
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,src2_id);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,regfile[src2_id],regfile[src2_id],regfile[dest_id]); 
				break;
		case XORI: 	result=regfile[src1_id] ^ immediate;
				regfile[dest_id]=result;			
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,immediate);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,regfile[src2_id],regfile[src2_id],regfile[dest_id]); 

				break;
		case OR: 	result=regfile[src1_id] | regfile[src2_id];
				regfile[dest_id]=result;	
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,src2_id);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,regfile[src2_id],regfile[src2_id],regfile[dest_id]); 

				break;
		case SLL:	result=regfile[src1_id]<<immediate;
				regfile[dest_id]=result;	
				sprintf(inst_display," %s %d %d %d",opstring,dest_id,src1_id,immediate);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and result are:%x %s %x %x %x",
					cpc,opstring,regfile[src1_id],immediate,regfile[dest_id]); 

				break;
		




		case BLEZ:	if(regfile[dest_id] <=0)
				{
				
				npc=cpc+8+immediate;
				}
				else
				npc=npc+8;	
				sprintf(inst_display," %s %d %d",opstring,dest_id,immediate);

				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and npc are:%x %s %x %x %x",
					cpc,opstring,regfile[dest_id],immediate,npc); 
				
				break;

		case BGTZ:	if(regfile[dest_id]>0)
				{
				
				npc=cpc+8+immediate;
				}
				else
				npc=npc+8;
				sprintf(inst_display," %s %d  %d",opstring,dest_id,immediate);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and npc are:%x %s %x %x %x",
					cpc,opstring,regfile[dest_id],immediate,npc); 
				
				break;
		case BLTZ:	if(regfile[dest_id] <0)
				{
				
				npc=cpc+8+immediate;
				}
				else
				npc=npc+8;	
				sprintf(inst_display," %s %d %d",opstring,dest_id,immediate);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and npc are:%x %s %x %x %x",
					cpc,opstring,regfile[dest_id],immediate,npc); 
				
				break;

		case BGEZ:	if(regfile[dest_id]>=0)
				{
				
				npc=cpc+8+immediate;
				}
				else
				npc=npc+8;
				sprintf(inst_display," %s %d %d",opstring,dest_id,immediate);
				sprintf(execution_str,"The PC,opcode, operand1,operand 2 and npc are:%x %s %x %x %x",
					cpc,opstring,regfile[src1_id],regfile[dest_id],npc); 
				
				break;
		
		case J:		npc=(cpc & 0xf0000000) | immediate;	
				sprintf(inst_display," %s %d",opstring,immediate);
				sprintf(execution_str,"The PC,opcode, operand1, and npc are:%x %s %x %x",
					cpc,opstring,immediate,npc); 
				
				break;
	
		


		case JAL:	npc=(cpc & 0xf0000000) | immediate;	
				sprintf(inst_display," %s %d",opstring,immediate);
				sprintf(execution_str,"The PC,opcode, operand1 and npc are:%x %s %x %x ",
					cpc,opstring,immediate,npc); 
				
				//regfile[31]=
				break;
		case JR: 	npc=regfile[dest_id];	
				sprintf(inst_display," %s %d",opstring,immediate);
				sprintf(execution_str,"The PC,opcode, operand1 and npc are:%x %s %x %x ",
					cpc,opstring,regfile[dest_id],npc); 
				
				break;

		case JALR:	regfile[31]=cpc+8;
				npc=regfile[dest_id]; 	
				sprintf(inst_display," %s %d",opstring,immediate);
				sprintf(execution_str,"The PC,opcode, operand1 and npc are:%x %s %x %x ",
					cpc,opstring,regfile[dest_id],npc); 
				     
				break;
		

		default:  printf("Should not come here\n");
			  exit(0);
	//	case JR:
	
	//	case JALR:

			
	}
	printf("%s\n",inst_display);	
	printf("%s\n",execution_str);	
	

	return npc;
}





long dec(char str[])//converts hedecimal string to decimal
{
	
	int temp=0;
	long result=0;
	int k=0;
	while(str[k] != '\0')
	{
        	result = result * 16;
        	if(str[k] >= '0' && str[k] <= '9')
        	{
        		    result = result + (str[k] - '0');
        	}
        	else
		{
			temp = chartoint(str[k]);
        		 result = result + temp;
        	}
		k++;
         }
return result;
}	


long  chartoint(int c)//function called by the function dec to convert hex to decimal
{
    char hex[] = "aAbBcCdDeEfF";
    int k=0;
    long result = 0;
    while(hex[k]!='\0' && result==0)
	{
        	if(hex[k] == c)
        	{
        	    result = 10 + (k / 2);
        	}
		k++;
	}
 
    return result;
}

int load_assembly(char* file_name)
{
	enum type_Flag
	{
		OPCODE,
		REG,
		IMM,
	};
		
	char line[100];
	int index=0;
	FILE *fp;
	fp= fopen(file_name,"r");//opening required file	

	int j=32,f=0,regNum=0,k=0,startOperand,withinOperand,endOperand;
	int addr,i=0,temp=0,p=0,q=0;
	long op_dec = 0;
	int dest_id,src1_id,src2_id,immediate=0;
	char opcode[10],imm[10];
	char reg[3][10], buffer[10];
	char type[30],address[10],data[10];	
	enum type_Flag typeFlag;
	char reg0[10],reg1[10],reg2[10];
	int imm_valid=0;
	if (fp!=NULL)//if file is not empty
	
	{	
		printf("File open succeeded\n");
		while (feof(fp)==0)//till it reaches the end of the file where it returns 1
		{	
			fgets(line,75,fp);
			j=0;
			while(line[j]!='\n')
			{
				printf("%c",line[j]);
				j++;
			}
			printf("\n");
			
			if(line[0] == '\n' || line[0] == '\0'){
			} else {

				get_substr(line,0,':',type);
					
			
				get_substr(line,0,':',type);
			
			
				if (!strcmp(type,"inst"))
				{
					

					get_substr(line,10,'	',address);
					j=32;
					f=0;
					regNum=0;
			
					strcpy(imm,"");
					strcpy(reg[2],"");
			
					typeFlag=OPCODE;
					startOperand = 1;
					endOperand = 0;
					withinOperand = 1;
	
					while(line[j]!='\n')
					{
						if(	line[j] == ' ' ||
							line[j] == ',' ||
							line[j] == '[' ||
							line[j] == ']' ){
	
							if(withinOperand)
							endOperand = 1;
							}
					
						if(	line[j] == '$'){
							startOperand = 1;
							typeFlag = REG;
							}
	
						if(	line[j] == 'x' ||
							line[j] == '#' ){
							startOperand = 1;
							typeFlag = IMM;
							}
					
	
						if(startOperand){
							f = 0;
							startOperand = 0;
							withinOperand = 1;
							}
	
						if(endOperand){
						buffer[f] = '\0';
						switch(typeFlag){
							case OPCODE: printf("Buffer: %s\n",buffer);strcpy(opcode,buffer); break;
							case REG: strcpy(reg[regNum],buffer); regNum++; break;
							case IMM: strcpy(imm,buffer); break;
						}
						endOperand = 0;
						withinOperand = 0;
						}	
	
						buffer[f] = line[j];
						f++;
						j++;
					
					}
					if(withinOperand){
						buffer[f]='\0';	
						switch(typeFlag){
							case OPCODE: strcpy(opcode,buffer); break;
							case REG: strcpy(reg[regNum],buffer); break;
							case IMM: strcpy(imm,buffer); imm_valid=1;break;
						}
					}
					i=0;
					op_dec=0;
					while(opcode[i]!='\0')
					{
						temp=opcode[i];
						op_dec=temp+(op_dec*1000);
						i++;
					}
					if (strlen(imm)!=0)
					{
						//p=1;
						//q=0;
						//while(imm[p]!='\0')
						//{
						//	imd[q]=imm[p];
						//	p++;
						//	q++;
						//};
						//imd[k]='\0';
						if(imm[0]=='x')
						{
							immediate=dec(&imm[1]); 
						}
						else
						{
							immediate=atoi(&imm[1]);
						}
					}	
					if(strlen(reg[0])!=0)
					{
						p=2;
						q=0;
						while(reg[0][p]!='\0')
				
						{
							reg0[q]=reg[0][p];
							p++;
							q++;
						};
						reg0[q]='\0';
						dest_id=atoi(reg0);
					}	
				
					if(strlen(reg[1])!=0)
					{
						p=2;
						q=0;
						while(reg[1][p]!='\0')
				
						{
							reg1[q]=reg[1][p];
							p++;
							q++;
						};
						reg1[q]='\0';
						src1_id=atoi(reg1);
					}
				
				
					if(strlen(reg[2])!=0)
					{
						p=2;
						q=0;
						while(reg[2][p]!='\0')
				
						{
							reg2[q]=reg[2][p];
							p++;
							q++;
						};
						reg2[q]='\0';
						src2_id=atoi(reg2);
					}
						
						
					printf("the parsed line is : opcode: %s dec_imm: %s register:%s %s %s op_dec: %ld\n",
						opcode,imm,reg[0],reg[1],reg[2],op_dec);
					//i++;
					printf("i am here");
					addr=dec(address);
	
					printf("address is %d\n",addr);
					index=addr/4;
					strcpy(memblock[index].op_str, opcode);
					memblock[index].immediate = immediate;
					memblock[index].dest_id = dest_id;
					memblock[index].src1_id = src1_id;
					memblock[index].src2_id = src2_id;
					memblock[index].op_dec = op_dec;
					memblock[index].imm_valid = imm_valid;
					memblock[index].data= 0;
						
				
		  		}
				else if(!strcmp(type,"idata"))
				{
					get_substr(line,21,'\n',data);
					get_substr(line,10,'\t',address);
					addr=dec(address);
					index=addr/4;
					printf("line: %s\n ",line);	
					printf("data: %s\n ",data);	
					printf("address: %s\n ",address);	
					memblock[index].data = dec(data);
					printf("data: %ld\n ",memblock[index].data);	
				}
			}
				
				
				
				
		}
			
			
	}
	for(k=0;k<2048;k++)
	{
		printf("the memory contents are: instruction: %s %d %d %d %d and data: %x dec_op:%ld in index %d\n",
			memblock[k].op_str,memblock[k].dest_id,
			memblock[k].src1_id,memblock[k].src2_id,
			memblock[k].immediate,memblock[k].data,
			memblock[k].op_dec,k);
			
	}
	return index;
	
}
char* get_substr(char* memblock_line,int location,char end_char,char* substr)
	{
		int  j=0,f=0;
		j=location;
		while(memblock_line[j]!=end_char)
		{
			substr[f]=memblock_line[j];
			j++;
			f++;
		}
		substr[f]='\0';
		return substr;
	
		
	}	
	 





			




		

		







